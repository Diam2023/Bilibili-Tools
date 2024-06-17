//
// Created by diam on 24-1-20.
//

#include "CqMessageManager.h"

#include <drogon/drogon.h>
#include <trantor/utils/Logger.h>

#include "CqConnectionPool.h"

void cq::CqMessageManager::registerHandler(cq::CqMessageHandlerType &&handler)
{
    callbacks.emplace_back(handler);
}

void cq::CqMessageManager::registerHandler(
    const cq::CqMessageHandlerType &handler)
{
    callbacks.push_back(handler);
}

void cq::CqMessageManager::messageIn(const cq::CqMessageData &data)
{
    std::lock_guard<std::mutex> lg(messageQueueMutex);
    messageQueue.push(data);
    messageQueueCondVar.notify_one();
}

void cq::CqMessageManager::messageIn(cq::CqMessageData &&data)
{
    std::lock_guard<std::mutex> lg(messageQueueMutex);
    messageQueue.push(data);
    messageQueueCondVar.notify_one();
}

cq::CqMessageManager::CqMessageManager()
    : workerThread(&CqMessageManager::worker, this)
{
}

[[noreturn]] void cq::CqMessageManager::worker()
{
    using namespace std::chrono_literals;

    while (true)
    {
        cq::CqMessageData data;
        {
            std::unique_lock<std::mutex> ul(messageQueueMutex);
            messageQueueCondVar.wait(ul, [this]() {
                return !messageQueue.empty();
            });
            data = messageQueue.front();
            messageQueue.pop();
        }  // Release Lock

        // Call Normal Handlers

        for (const auto &handler : callbacks)
        {
            handler(data);
        }

        std::this_thread::sleep_for(1ms);
    }
}

void cq::CqMessageManager::messageOut(const cq::CqMessageData &data)
{
    // TODO use worker thread
    auto ptr = cq::CqConnectionPool::getInstance().getOutPtr(data.first);
    if (ptr)
    {
        LOG_DEBUG << data.second.toStyledString();
        ptr->send(data.second.toStyledString());
    }
    else
    {
        LOG_WARN << "Send Error!";
    }
}
