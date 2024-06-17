/**
 * @file CqConnectionPool.h
 * @author Diam (monoluths-uni@outlook.com)
 * @brief
 * @version 0.1-alpha
 * @date 2024-06-16
 *
 * @copyright Copyright (c) 2024 Diam
 *
 */
#ifndef CQCONNECTIONPOOL_H
#define CQCONNECTIONPOOL_H

#include <drogon/WebSocketConnection.h>
#include <condition_variable>

namespace cq
{

// Bot id and Bot ws ptr
using CqConnection = std::pair<std::string, drogon::WebSocketConnectionPtr>;

/**
 * CqHttp WebSocket Connection Pool
 */
class CqConnectionPool
{
  private:
    std::mutex poolMutex;

    std::condition_variable poolCondVar;

    std::vector<CqConnection> connectionPool;

  public:
    CqConnectionPool() = default;

    void addConnection(const std::string &,
                       const drogon::WebSocketConnectionPtr &);

    void removeConnection(const drogon::WebSocketConnectionPtr &);

    std::string getId(const drogon::WebSocketConnectionPtr &);

    /**
     * @brief 获取一个在线的机器人ID
     *
     * @return std::string 在线机器人ID
     */
    std::string getOnlineBotId();

    /**
     * get outPtr use bot id
     * @param botId bot id
     * @return bot ws connetion ptr
     */
    drogon::WebSocketConnectionPtr getOutPtr(const std::string &botId);

    static CqConnectionPool &getInstance()
    {
        static CqConnectionPool pool;
        return pool;
    }
};

}  // namespace cq

#endif  // CQCONNECTIONPOOL_H
