#include "CqWebsocket.h"
#include <json/value.h>
#include <trantor/utils/Logger.h>
#include <memory>

#include "CqConnectionPool.h"

#include "CqMessageManager.h"

void CqWebsocket::handleNewMessage(const WebSocketConnectionPtr &wsConnPtr,
                                   std::string &&message,
                                   const WebSocketMessageType &type)
{
    Json::Value tree;
    Json::String err;
    Json::CharReaderBuilder reader;
    std::unique_ptr<Json::CharReader> const json_read(reader.newCharReader());
    json_read->parse(message.c_str(),
                     message.c_str() + message.length(),
                     &tree,
                     &err);

    if ((type == WebSocketMessageType::Text) && err.empty())
    {
        auto id = cq::CqConnectionPool::getInstance().getId(wsConnPtr);
        auto msg = std::make_shared<Json::Value>(tree);
        // Load Manager
        cq::CqMessageManager::getInstance().messageIn({id, msg});
    }
}

void CqWebsocket::handleNewConnection(const HttpRequestPtr &req,
                                      const WebSocketConnectionPtr &wsConnPtr)
{
    auto qqId = req->getHeader("X-Self-ID");
    if (qqId.empty())
    {
        wsConnPtr->forceClose();
        LOG_INFO << "WS Non QQ ID, Close Connection";
        return;
    }

    LOG_INFO << "WS new bot";
    cq::CqConnectionPool::getInstance().addConnection(qqId, wsConnPtr);
}

void CqWebsocket::handleConnectionClosed(
    const WebSocketConnectionPtr &wsConnPtr)
{
    auto id = cq::CqConnectionPool::getInstance().getId(wsConnPtr);
    if (id.empty())
    {
        LOG_WARN << "Close Err WS handle";
    }
    else
    {
        LOG_WARN << "WS bot: " << id << " leave";
    }
    cq::CqConnectionPool::getInstance().removeConnection(wsConnPtr);
}
