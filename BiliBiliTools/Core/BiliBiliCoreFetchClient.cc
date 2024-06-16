#include "BiliBiliCoreFetchClient.h"

#include <drogon/HttpTypes.h>
#include <trantor/utils/Logger.h>

#include <exception>
#include <future>
#include <iostream>
#include <memory>
#include <string>

#ifdef __linux__
#include <sys/socket.h>
#include <netinet/tcp.h>
#endif

bilibili::api::FetchClient::FetchClient()
{
    if (!clientInstance)
    {
        clientInstance =
            drogon::HttpClient::newHttpClient(std::string(bilibiliApiUrl));
        clientInstance->setSockOptCallback([](int fd) {
        // std::cout << "setSockOptCallback:" << fd << std::endl;
#ifdef __linux__
            int optval = 10;
            ::setsockopt(fd,
                         SOL_TCP,
                         TCP_KEEPCNT,
                         &optval,
                         static_cast<socklen_t>(sizeof optval));
            ::setsockopt(fd,
                         SOL_TCP,
                         TCP_KEEPIDLE,
                         &optval,
                         static_cast<socklen_t>(sizeof optval));
            ::setsockopt(fd,
                         SOL_TCP,
                         TCP_KEEPINTVL,
                         &optval,
                         static_cast<socklen_t>(sizeof optval));
#endif
        });
    }
}

std::shared_ptr<bilibili::model::UserInfo> bilibili::api::FetchClient::
    fetchUserInfoByUserId(const std::string &id_)
{
    LOG_ERROR_IF(!clientInstance) << "Error Instance";
    LOG_INFO << "Fetch UserInfo with: " << id_;
    auto req = drogon::HttpRequest::newHttpRequest();

    req->setMethod(drogon::Get);
    req->setPath(std::string(bilibiliUserInfoApiPath));
    req->setParameter("uid", id_);

    auto prom = std::make_shared<
        std::promise<std::shared_ptr<bilibili::model::UserInfo>>>();
    clientInstance->sendRequest(
        req,
        [prom](drogon::ReqResult result,
               const drogon::HttpResponsePtr &response) {
            if (result != drogon::ReqResult::Ok)
            {
                prom->set_exception(std::make_exception_ptr(
                    FetchClientException("Err Request User Info")));
                return;
            }

            if (response->statusCode() != drogon::k200OK)
            {
                LOG_INFO << response->body();
                prom->set_exception(std::make_exception_ptr(
                    FetchClientException("Response Err")));
                return;
            }

            try
            {
                auto jsonObj = response->getJsonObject();
                auto resultCode = (*jsonObj)["code"].asInt();
                auto resultName =
                    (*jsonObj)["data"]["info"]["uname"].asString();
                if (resultCode == 0 && !resultName.empty())
                {
                    prom->set_value(std::make_shared<model::UserInfo>(
                        (*jsonObj)["data"]["info"]["uid"].asString(),
                        (*jsonObj)["data"]["room_id"].asString(),
                        (*jsonObj)["data"]["info"]["uname"].asString(),
                        (*jsonObj)["data"]["info"]["face"].asString(),
                        (*jsonObj)["data"]["exp"]["master_level"]["level"]
                            .asUInt()));
                }
                else
                {
                    prom->set_exception(std::make_exception_ptr(
                        FetchClientException("Not Found")));
                }
            }
            catch (const std::exception &e)
            {
                prom->set_exception(
                    std::make_exception_ptr(std::current_exception()));
            }
        });
    return prom->get_future().get();
}

std::shared_ptr<bilibili::model::RoomInfo> bilibili::api::FetchClient::
    fetchRoomInfoByRoomId(const std::string &id_)
{
    LOG_ERROR_IF(!clientInstance) << "Error Instance";
    LOG_INFO << "Fetch RoomInfo with: " << id_;
    auto req = drogon::HttpRequest::newHttpRequest();

    req->setMethod(drogon::Get);
    req->setPath(std::string(bilibiliRoomInfoApiPath));
    req->setParameter("room_id", id_);

    auto prom = std::make_shared<
        std::promise<std::shared_ptr<bilibili::model::RoomInfo>>>();
    clientInstance->sendRequest(
        req,
        [prom](drogon::ReqResult result,
               const drogon::HttpResponsePtr &response) {
            if (result != drogon::ReqResult::Ok)
            {
                prom->set_exception(std::make_exception_ptr(
                    FetchClientException("Err Request Room Info")));
                return;
            }

            try
            {
                auto jsonObj = response->getJsonObject();
                auto resultCode = (*jsonObj)["code"].asInt();
                if (resultCode == 0)
                {
                    auto liveStatus = (*jsonObj)["data"]["live_status"].asInt();
                    prom->set_value(std::make_shared<model::RoomInfo>(
                        (*jsonObj)["data"]["room_id"].asString(),
                        (*jsonObj)["data"]["uid"].asString(),
                        liveStatus == 0
                            ? bilibili::model::RoomInfo::LiveStatus::OffLine
                            : (liveStatus == 1
                                   ? model::RoomInfo::LiveStatus::OnLine
                                   : model::RoomInfo::LiveStatus::Carousel),
                        (*jsonObj)["data"]["area_name"].asString(),
                        (*jsonObj)["data"]["title"].asString(),
                        (*jsonObj)["data"]["keyframe"].asString()));
                }
                else
                {
                    prom->set_exception(std::make_exception_ptr(
                        FetchClientException("Not Found")));
                }
            }
            catch (const std::exception &e)
            {
                prom->set_exception(
                    std::make_exception_ptr(std::current_exception()));
            }
        });
    return prom->get_future().get();
}
