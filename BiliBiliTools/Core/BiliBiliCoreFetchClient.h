#ifndef BILIBILICOREFETCHCLIENT_H
#define BILIBILICOREFETCHCLIENT_H

#include <drogon/HttpClient.h>
#include <memory>
#include <string>

#include "Common.h"

#include "BiliBiliUserInfo.h"
#include "BiliBiliRoomInfo.h"

namespace bilibili::api
{

class FetchClient
{
  public:
    drogon::HttpClientPtr clientInstance;

    constexpr static std::string_view bilibiliApiUrl =
        "https://api.live.bilibili.com";

    constexpr static std::string_view bilibiliUserInfoApiPath =
        "/live_user/v1/Master/info";

    constexpr static std::string_view bilibiliRoomInfoApiPath =
        "/room/v1/Room/get_info";

    FetchClient();
    FetchClient(FetchClient &&) = delete;
    FetchClient(const FetchClient &) = delete;
    FetchClient &operator=(FetchClient &&) = delete;
    FetchClient &operator=(const FetchClient &) = delete;
    ~FetchClient() = default;

    /**
     * @brief 阻塞获取UserInfo
     *
     * @param id_ 用户id
     * @return std::shared_ptr<bilibili::model::UserInfo> 用户信息
     */
    std::shared_ptr<bilibili::model::UserInfo> fetchUserInfoByUserId(
        const std::string &id_);

    /**
     * @brief 阻塞获取RoomInfo
     *
     * @param id_ 房间id
     * @return std::shared_ptr<bilibili::model::RoomInfo> 房间信息
     */
    std::shared_ptr<bilibili::model::RoomInfo> fetchRoomInfoByRoomId(
        const std::string &id_);

  private:
    SINGLETON_INTERFACE(FetchClient);
};

class FetchClientException final : public std::exception
{
  public:
    const char *what() const noexcept override
    {
        return message_.data();
    }

    FetchClientException(const std::string &message) : message_(message)
    {
    }

    FetchClientException(std::string &&message) : message_(std::move(message))
    {
    }

    FetchClientException() = delete;

  private:
    std::string message_;
};

}  // namespace bilibili::api

#endif  // BILIBILICOREFETCHCLIENT_H
