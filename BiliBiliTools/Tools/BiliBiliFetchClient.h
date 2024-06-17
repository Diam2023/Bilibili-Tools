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

/**
 * @brief 简化获取房间直播状态
 *
 * @param id_ 用户id
 * @return bilibili::model::RoomInfo::LiveStatus
 */
bilibili::model::RoomInfo::LiveStatus FetchRoomStatusByUserId(
    const std::string &id_);

/**
 * @brief 使用用户号获取房间号
 *
 * @param id_ 用户id
 * @return std::string
 */
std::string FetchRoomIdByUserId(const std::string &id_);

/**
 * @brief 使用房间号获取直播状态
 *
 * @param id_ 用户id
 * @return bilibili::model::RoomInfo::LiveStatus
 */
bilibili::model::RoomInfo::LiveStatus FetchRoomStatusByRoomId(
    const std::string &id_);

}  // namespace bilibili::api

#endif  // BILIBILICOREFETCHCLIENT_H
