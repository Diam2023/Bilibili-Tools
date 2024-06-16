//
// Created by diam on 24-1-19.
//

#include "CqConnectionPool.h"

namespace cq
{
void CqConnectionPool::addConnection(
    const std::string &id,
    const drogon::WebSocketConnectionPtr &connectionPtr)
{
    std::lock_guard<std::mutex> lg(poolMutex);
    connectionPool.emplace_back(id, connectionPtr);
    poolCondVar.notify_one();
}

void CqConnectionPool::removeConnection(
    const drogon::WebSocketConnectionPtr &connectionPtr)
{
    if (connectionPool.empty())
    {
        return;
    }
    std::unique_lock<std::mutex> ul(poolMutex);
    connectionPool.erase(
        std::remove_if(connectionPool.begin(),
                       connectionPool.end(),
                       [connectionPtr](const auto &data) -> bool {
                           return (connectionPtr == data.second);
                       }),
        connectionPool.end());
}

std::string CqConnectionPool::getId(
    const drogon::WebSocketConnectionPtr &connectionPtr)
{
    const auto result = std::find_if(connectionPool.cbegin(),
                                     connectionPool.cend(),
                                     [connectionPtr](const auto &conn) -> bool {
                                         return (connectionPtr == conn.second);
                                     });
    if (result != connectionPool.end())
    {
        return (*result).first;
    }

    return "";
}

drogon::WebSocketConnectionPtr CqConnectionPool::getOutPtr(
    const std::string &botId)
{
    const auto result = std::find_if(connectionPool.cbegin(),
                                     connectionPool.cend(),
                                     [botId](const auto &conn) -> bool {
                                         return (botId == conn.first);
                                     });
    if (result != connectionPool.end())
    {
        return (*result).second;
    }

    return {};
}
}  // namespace cq
