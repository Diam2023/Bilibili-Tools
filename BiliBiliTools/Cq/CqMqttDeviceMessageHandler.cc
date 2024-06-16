//
// Created by diam on 24-1-19.
//

#include "CqMqttDeviceMessageHandler.h"
#include "Tools.h"

#include "DeviceApi.h"

#include "Device.h"
#include "SubscribeMap.h"
#include "Topic.h"

#include "Common.h"
#include "CqMessageManager.h"

#include <string>
#include <vector>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon::nosql;
using namespace drogon_model::iot_server;

cq::CqMqttDeviceMessageHandler::CqMqttDeviceMessageHandler() = default;

void cq::CqMqttDeviceMessageHandler::handler(const mqtt::MqttData &data)
{
    auto result =
        getDeviceSerialNumberAndSubTopic(data.first.name().toStdString());
    if (!result.first.empty())
    {
        auto deviceProm = std::make_shared<std::promise<Device>>();
        drogon::app().getLoop()->queueInLoop([deviceProm, result]() {
            auto dbClientPtr = drogon::app().getDbClient();
            Mapper<Device> deviceMapper(dbClientPtr);
            deviceMapper.findOne(
                Criteria(Device::Cols::_device_sn,
                         CompareOperator::EQ,
                         result.first),
                [deviceProm](const Device &r) { deviceProm->set_value(r); },
                [deviceProm](const auto &e) {
                    deviceProm->set_exception(std::current_exception());
                });
        });

        uint32_t deviceId;
        Device device;
        try
        {
            device = deviceProm->get_future().get();
            deviceId = *device.getDeviceId();
        }
        catch (const DrogonDbException &e)
        {
            // Not Found
            return;
        }

        auto topicProm = std::make_shared<std::promise<Topic>>();
        drogon::app().getLoop()->queueInLoop([topicProm, result, deviceId]() {
            auto dbClientPtr = drogon::app().getDbClient();
            Mapper<Topic> topicMapper(dbClientPtr);
            topicMapper.findOne(
                Criteria(Topic::Cols::_target_device_id,
                         CompareOperator::EQ,
                         deviceId) &&
                    Criteria(Topic::Cols::_topic_name,
                             CompareOperator::EQ,
                             result.second),
                [topicProm](const Topic &r) { topicProm->set_value(r); },
                [topicProm](const auto &e) {
                    topicProm->set_exception(std::current_exception());
                });
        });

        uint32_t topicId;
        Topic topic;
        try
        {
            topic = topicProm->get_future().get();
            topicId = *topic.getTopicId();
        }
        catch (const DrogonDbException &e)
        {
            // Not Found
            return;
        }

        auto userIdListProm =
            std::make_shared<std::promise<std::vector<uint32_t>>>();

        drogon::app().getLoop()->queueInLoop(
            [userIdListProm, deviceId, topicId]() {
                auto dbClientPtr = drogon::app().getDbClient();
                Mapper<SubscribeMap> subsMapper(dbClientPtr);
                subsMapper.findBy(
                    Criteria(SubscribeMap::Cols::_target_device_id,
                             CompareOperator::EQ,
                             deviceId) &&
                        Criteria(SubscribeMap::Cols::_target_topic_id,
                                 CompareOperator::EQ,
                                 topicId),
                    [userIdListProm](const auto &r) {
                        std::vector<uint32_t> uidList;
                        for (const SubscribeMap &subs : r)
                        {
                            uidList.push_back(*subs.getTargetUserId());
                        }
                        userIdListProm->set_value(uidList);
                    },
                    [userIdListProm](const auto &e) {
                        userIdListProm->set_exception(std::current_exception());
                    });
            });

        std::vector<uint32_t> userIdList;
        try
        {
            userIdList = userIdListProm->get_future().get();
        }
        catch (const DrogonDbException &e)
        {
            // Not Found
            return;
        }

        if (userIdList.empty())
        {
            // NON Data Tp Send
            return;
        }
        auto sendVec = std::make_shared<
            std::vector<std::pair<std::string, std::string>>>();

        for (auto uid : userIdList)
        {
            // redis find
            auto qidProm = std::make_shared<std::promise<std::string>>();
            drogon::app().getLoop()->queueInLoop([uid, qidProm]() {
                auto redisClientPtr = drogon::app().getRedisClient();
                redisClientPtr->execCommandAsync(
                    [qidProm](const RedisResult &r) {
                        if (r.type() == RedisResultType::kString)
                        {
                            qidProm->set_value(r.asString());
                        }
                    },
                    [](const RedisException &e) {},
                    "get %s%d",
                    UQ_PREFIX,
                    uid);
            });

            std::string qid;
            try
            {
                qid = qidProm->get_future().get();
            }
            catch (const RedisException &e)
            {
                // Err Found
                continue;
            }

            auto bidProm = std::make_shared<std::promise<std::string>>();
            drogon::app().getLoop()->queueInLoop([qid, bidProm]() {
                auto redisClientPtr = drogon::app().getRedisClient();
                redisClientPtr->execCommandAsync(
                    [qid, bidProm](const RedisResult &r) {
                        if (r.type() == RedisResultType::kString)
                        {
                            bidProm->set_value(r.asString());
                        }
                    },
                    [](const RedisException &e) {},
                    "get %s%s",
                    QB_PREFIX,
                    qid.c_str());
            });

            std::string bid;
            try
            {
                bid = bidProm->get_future().get();
            }
            catch (const RedisException &e)
            {
                // Err Found
                continue;
            }
            sendVec->emplace_back(bid, qid);
        }

        if (!sendVec->empty())
        {
            for (const auto &res : *sendVec)
            {
                cq::CqMessageManager::getInstance().messageOut(
                    std::get<0>(res),
                    std::get<1>(res),
                    convertVisual(data, *device.getDeviceName()));
            }
        }
        else
        {
            LOG_INFO << "No User To sub";
        }
    }
    else
    {
        LOG_INFO << "No User To sub";
    }
}

std::string cq::CqMqttDeviceMessageHandler::convertVisual(
    const mqtt::MqttData &data,
    const std::string &deviceName)
{
    // Just For Test
    std::stringstream ss;
    // TODO Get Alias And Device Name
    if (data.second["status"].isBool())
    {
        if (data.second["status"].asBool())
        {
            ss << deviceName << " Opened";
        }
        else
        {
            ss << deviceName << " Closed";
        }
    }
    else
    {
        ss << deviceName << std::endl << " JSON: " << std::endl;
        ss << data.second.toStyledString();
    }
    return ss.str();
}
