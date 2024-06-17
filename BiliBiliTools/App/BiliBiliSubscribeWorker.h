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
#include <cstdint>
#include <string>
#include <thread>
#include <tuple>
#include <vector>
#include "Common.h"
#include "CqCommandHandler.h"
#include "priority_timer.h"

namespace bilibili
{

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
    void notify(std::tuple<cq::ChatSenderIdType,
                           std::string,
                           cq::ChatMessageType,
                           uint32_t> msg,
                bool status);

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

    // qqid roomid qqtid_type
    std::vector<std::tuple<cq::ChatSenderIdType,
                           std::string,
                           cq::ChatMessageType,
                           uint32_t>>
        cache;
};

}  // namespace bilibili
