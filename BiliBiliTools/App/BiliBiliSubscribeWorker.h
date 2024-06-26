/**
 * @file BiliBiliFetchWorker.h
 * @author Diam (monoluths-uni@outlook.com)
 * @brief 用于定时获取直播状态
 * @version 0.1-alpha
 * @date 2024-06-17
 *
 * @copyright Copyright (c) 2024 Diam
 *
 */
#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <thread>
#include <tuple>
#include <vector>
#include "BiliBiliUserInfo.h"
#include "Common.h"
#include "CqCommandHandler.h"
#include "priority_timer.h"

namespace bilibili
{

/**
 * @brief qqid roomid qqtid_type UserInfo
 */
using NotifyMessageType =
    std::tuple<cq::ChatSenderIdType,
               std::string,
               cq::ChatMessageType,
               std::shared_ptr<bilibili::model::UserInfo>>;

/**
 * @brief 订阅工人
 *
 */
class SubscribeWorker
{
  public:
    SubscribeWorker();
    SubscribeWorker(SubscribeWorker &&) = delete;
    SubscribeWorker(const SubscribeWorker &) = delete;
    SubscribeWorker &operator=(SubscribeWorker &&) = delete;
    SubscribeWorker &operator=(const SubscribeWorker &) = delete;
    ~SubscribeWorker() = default;

    /**
     * @brief 找机器人通知去
     *
     * @param msg
     * @param status
     */
    void notify(NotifyMessageType notifyMsg, bool status);

    /**
     * @brief 工作线程
     */
    void work();

    /**
     * @brief 更新缓存
     */
    void updateCache();

    void NotifyTimerCallback();

    SINGLETON_INTERFACE(SubscribeWorker);

  private:
    std::thread workThread;
    MONO_PriorityTimerQueue_t *mainTimerQueue;

    std::atomic<bool> runStatus = false;

    /**
     * @brief 更新则为true
     *
     */
    std::atomic<bool> updateCacheFlag = false;

    /**
     * @brief 用于储存要获取的房间
     * roomid
     */
    std::vector<std::shared_ptr<std::string>> roomListCache;
};

}  // namespace bilibili
