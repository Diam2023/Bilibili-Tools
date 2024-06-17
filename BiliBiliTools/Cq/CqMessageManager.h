/**
 * @file CqMessageManager.h
 * @author Diam (monoluths-uni@outlook.com)
 * @brief
 * @version 0.1-alpha
 * @date 2024-06-16
 *
 * @copyright Copyright (c) 2024 Diam
 *
 */

#pragma once

#include "Common.h"
#include "CqMessageHandler.h"

#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace cq
{
/**
 * To Filter, Handler WS Message
 */
class CqMessageManager
{
  private:
    /**
     * Callbacks
     */
    std::vector<CqMessageHandlerType> callbacks;

    // Message Queue
    std::queue<CqMessageData> messageQueue;
    std::mutex messageQueueMutex;
    std::condition_variable messageQueueCondVar;

    //        // Message out queue
    //        std::queue<CqMessageData> messageOutQueue;
    //        std::mutex messageOutQueueMutex;
    //        std::condition_variable messageOutQueueCondVar;

    std::thread workerThread;

  public:
    CqMessageManager();

    /**
     * @brief 接收到消息的处理
     *
     * @param data
     */
    void messageIn(const cq::CqMessageData &data);

    void messageIn(cq::CqMessageData &&data);

    /**
     * @brief 发送信息
     *
     * @param data 信息结构
     */
    void messageOut(const cq::CqMessageData &data);

    /**
     * Do Not Use This Api in multi-thread
     * @param handler
     */
    void registerHandler(const CqMessageHandlerType &handler);

    void registerHandler(CqMessageHandlerType &&handler);

    // Do not call this
    [[noreturn]] void worker();

    SINGLETON_INTERFACE(CqMessageManager);
};

}  // namespace cq
