#include <drogon/HttpAppFramework.h>
#include <drogon/drogon.h>
#include <trantor/utils/Logger.h>
#include <thread>

#include "BiliBiliRoomInfo.h"
#include "LiveFetch.h"

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

int main()
{
    // drogon::app().addListener("0.0.0.0", 5555);
    drogon::app().loadConfigFile("../../config.json");

    auto thr = std::thread([]() {
        try
        {
            auto status = bilibili::tools::FetchRoomStatusByUserId("689460774");
            if (status == bilibili::model::RoomInfo::LiveStatus::OnLine)
            {
                LOG_INFO << "直播中";
            }
            else
            {
                LOG_INFO << "未直播";
            }
        }
        catch (const std::exception &e)
        {
            LOG_WARN << e.what();
        }
    });

    runServer();

    return 0;
}
