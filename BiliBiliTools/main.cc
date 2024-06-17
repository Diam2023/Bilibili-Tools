#include <drogon/HttpAppFramework.h>
#include <drogon/drogon.h>
#include <trantor/utils/Logger.h>

#include "BiliBiliSubscribeWorker.h"
#include "CqChatMessageHandler.h"
#include "CqCommandHandler.h"
#include "CqMessageManager.h"

#ifdef __linux__
#include <sys/socket.h>
#include <netinet/tcp.h>
#endif

// Handler Exit Signal
static void exitSignalHandler()
{
    // Handle Exit Signal
    LOG_WARN << "Server Exiting...";
    drogon::app().quit();
}

static void runServer()
{
    using namespace std::chrono_literals;

    drogon::app().setIntSignalHandler(&exitSignalHandler);
    drogon::app().setTermSignalHandler(&exitSignalHandler);

    drogon::app().run();
}

int main(int argc, const char **argv)
{
    if (argc >= 2)
    {
        LOG_INFO << "LOAD CONFIG FROM ARG: " << argv[1];
        drogon::app().loadConfigFile(argv[1]);
    }
    else
    {
        // drogon::app().addListener("0.0.0.0", 5555);
        drogon::app().loadConfigFile("../../config.json");
    }

    // 初始化ComandHandler 生成缓存等
    cq::CqCommandHandler::getInstance().init();
    // 注册ComandHandler到信息工厂
    cq::CqMessageManager::getInstance().registerHandler(
        cq::ChatMessageHandler::getInstance());
    bilibili::SubscribeWorker::getInstance();

    runServer();

    return 0;
}
