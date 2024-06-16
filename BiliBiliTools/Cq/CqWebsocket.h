#pragma once

#include <drogon/WebSocketController.h>

using namespace drogon;

class CqWebsocket : public drogon::WebSocketController<CqWebsocket>
{
  public:
    void handleNewMessage(const WebSocketConnectionPtr &,
                          std::string &&,
                          const WebSocketMessageType &) override;

    void handleNewConnection(const HttpRequestPtr &,
                             const WebSocketConnectionPtr &) override;

    void handleConnectionClosed(const WebSocketConnectionPtr &) override;

    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/qq/receive", "drogon::LocalHostFilter");
    WS_PATH_LIST_END
};
