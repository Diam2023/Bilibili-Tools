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

    void messageIn(const cq::CqMessageData &data);

    void messageIn(cq::CqMessageData &&data);

    // TODO 添加群组发送消息
    /**
     * Message output
     * @param botId
     * @param targetId
     * @param message
     */
    void messageOut(const std::string &botId,
                    const std::string &targetId,
                    const std::string &message);

    /**
     * Do Not Use This Api in multi-thread
     * @param handler
     */
    void registerHandler(const CqMessageHandlerType &handler);

    // Do not call this
    [[noreturn]] void worker();

    static CqMessageManager &getInstance()
    {
        static CqMessageManager manager;
        return manager;
    }
};

}  // namespace cq
