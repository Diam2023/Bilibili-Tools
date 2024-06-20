#include "BiliBiliSubscribeWorker.h"
#include <drogon/orm/Mapper.h>
#include <trantor/utils/Logger.h>

#include <exception>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include "BiliBiliFetchClient.h"
#include "BiliBiliRoomInfo.h"
#include "CqCommandHandler.h"
#include "CqConnectionPool.h"
#include "CqMessageManager.h"
#include "LiveNotify.h"
#include "LiveSubscribe.h"
#include "drogon/HttpAppFramework.h"
#include "drogon/nosql/RedisResult.h"
#include "drogon/orm/Criteria.h"
#include "priority_timer.h"

static std::mutex queueMutex;

using namespace drogon::orm;
using namespace drogon_model::bilibili_database;
using namespace drogon::nosql;

extern "C" bool MONO_TryLockTimerQueue(MONO_PriorityTimerQueue_t *queue_)
{
    if (queueMutex.try_lock())
    {
        return true;
    }

    return false;
}

extern "C" void MONO_UnlockTimerQueue(MONO_PriorityTimerQueue_t *queue_)
{
    queueMutex.unlock();
}

void *UpdateFetchTask(void *arg)
{
    auto roomId = *(std::string *)arg;

    LOG_INFO << "Ontime: " << roomId;

    // 查询Notfy内所有subs为roomId的通知对象, 并通知

    try
    {
        auto status = bilibili::api::FetchRoomStatusByRoomId(roomId);
        auto statusId = std::make_shared<int>(
            status == bilibili::model::RoomInfo::LiveStatus::OnLine ? 1 : 0);
        drogon::app().getRedisClient()->execCommandAsync(
            [roomId, statusId](const RedisResult &r) {
                // 当直播的状态与数据库中不同时就响应
                if ((r.type() != RedisResultType::kString) ||
                    ((r.type() == RedisResultType::kString) &&
                     (r.asString() != std::to_string(*statusId))))
                {
                    drogon::app().getRedisClient()->execCommandAsync(
                        [](const RedisResult &r) {
                            // TODO Check Successful
                        },
                        [](const auto &e) { LOG_WARN << "Error In Redis Get"; },
                        "set %s%s %d",
                        LIVE_STATUS_CACHE_PREFIX,
                        roomId.c_str(),
                        *statusId);

                    Mapper<LiveNotify> liveNotifyMapper(
                        drogon::app().getDbClient());
                    std::vector<LiveNotify> liveNotifyList;
                    try
                    {
                        liveNotifyList = liveNotifyMapper.findBy(
                            Criteria(LiveNotify::Cols::_subscribe_target,
                                     CompareOperator::EQ,
                                     roomId));
                    }
                    catch (const std::exception &)
                    {
                        LOG_INFO << roomId << " Has No Notify Target";
                        return;
                    }

                    for (auto &liveNotify : liveNotifyList)
                    {
                        // qqid roomid qqtid_type
                        bilibili::NotifyMessageType notifyMessage{
                            liveNotify.getValueOfNotifyTarget(),
                            roomId,
                            (liveNotify.getValueOfTargetType() == "PRIVATE"
                                 ? cq::ChatMessageType::Private
                                 : cq::ChatMessageType::Group)};
                        // Notify!
                        bilibili::SubscribeWorker::getInstance().notify(
                            notifyMessage, *statusId);
                    }
                }
            },
            [](const auto &e) { LOG_WARN << "Error In Redis Get"; },
            "get %s%s",
            LIVE_STATUS_CACHE_PREFIX,
            roomId.c_str());
    }
    catch (const std::exception &)
    {
        LOG_WARN << "Fetch: " << roomId << " Err";
    }

    return nullptr;
}

bilibili::SubscribeWorker::SubscribeWorker()
    : workThread(&SubscribeWorker::work, this),
      mainTimerQueue(MONO_CreatePriorityQueue())
{
    LOG_FATAL_IF(mainTimerQueue == nullptr) << "ERROR Queue Init";
    // update first
    updateCacheFlag = true;
    runStatus = true;
}

static char tempString[512];

void bilibili::SubscribeWorker::notify(NotifyMessageType notifyMsg, bool status)
{
    auto onLineBotId = cq::CqConnectionPool::getInstance().getOnlineBotId();
    if (onLineBotId.empty())
    {
        LOG_WARN << "Error Online Bot";
        return;
    }

    // qqid roomid qqtid_type
    // onLineBotId

    auto roomId = std::get<1>(notifyMsg);
    // message
    std::string outputMessage;
    std::stringstream tempSS;

    auto boardcastMessages =
        drogon::app().getCustomConfig()["cqhttp"]["broadcast_message"];

    // bilibili::model::RoomInfo;
    auto roomInfo =
        api::FetchClient::getInstance().fetchRoomInfoByRoomId(roomId);
    auto userInfo = api::FetchClient::getInstance().fetchUserInfoByUserId(
        roomInfo->getUserId());
    if (status)
    {
        // Notify Live

        // TODO 优化

        // TODO Use C++20 fmt
        memset(tempString, 0, sizeof(tempString));
        sprintf(tempString,
                boardcastMessages["live_start"][0].asCString(),
                userInfo->getName().c_str());
        tempSS << std::string(tempString) << std::endl;
        memset(tempString, 0, sizeof(tempString));
        sprintf(tempString,
                boardcastMessages["live_start"][1].asCString(),
                "https://live.bilibili.com/");
        tempSS << tempString << userInfo->getRoomId().c_str();
        // TODO Add Frame Img
    }
    else
    {
        // Live Ofline
        memset(tempString, 0, sizeof(tempString));
        sprintf(tempString,
                boardcastMessages["live_end"][0].asCString(),
                userInfo->getName().c_str());
        tempSS << std::string(tempString);
    }
    outputMessage = tempSS.str();

    auto targetId = std::get<0>(notifyMsg);
    auto targetType = std::get<2>(notifyMsg);
    cq::CqMessageManager::getInstance().messageOut(
        cq::CqCommandHandler::MakeCqMesageData(
            {onLineBotId, targetId, targetId, outputMessage, targetType}));

    // msg
}

void bilibili::SubscribeWorker::work()
{
    using namespace std::chrono_literals;

    while (true)
    {
        std::this_thread::sleep_for(1s);

        if (runStatus)
        {
            // TODO 使用最新版本
            if (TimerTickHandler(mainTimerQueue) > 0)
            {
                MONO_QueueTaskInfo(mainTimerQueue);
            }
        }

        if (updateCacheFlag)
        {
            ClearTimerQueue(mainTimerQueue);

            roomListCache.clear();

            Mapper<LiveSubscribe> liveSubscribeMapper(
                drogon::app().getDbClient());

            std::vector<LiveSubscribe> liveSubscribeList;

            try
            {
                liveSubscribeList = liveSubscribeMapper.findAll();
            }
            catch (const DrogonDbException &e)
            {
                LOG_WARN << "Exception when find LiveSubscribe";
            }
            for (const auto &liveSubscribe : liveSubscribeList)
            {
                if (liveSubscribe.getValueOfCheckTimer() == 0)
                {
                    LOG_ERROR << "Error Check Timer: "
                              << liveSubscribe.getValueOfSubscribeTarget();
                    continue;
                }
                // Rule
                auto res = roomListCache.emplace_back(
                    liveSubscribe.getValueOfSubscribeTarget());
                NewTimerTask(mainTimerQueue,
                             UpdateFetchTask,
                             liveSubscribe.getValueOfCheckTimer(),
                             (void *)(&res));
            }
            updateCacheFlag = false;

            EnableTimerQueue(mainTimerQueue);
        }
    }
}

void bilibili::SubscribeWorker::updateCache()
{
    updateCacheFlag = true;
}
