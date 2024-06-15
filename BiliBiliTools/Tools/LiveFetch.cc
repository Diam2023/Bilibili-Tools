#include "LiveFetch.h"
#include <drogon/nosql/RedisException.h>
#include <string>

#include "BiliBiliCoreFetchClient.h"

bilibili::model::RoomInfo::LiveStatus bilibili::tools::FetchRoomStatusByUserId(
    const std::string &id_)
{
    auto userInfo =
        bilibili::api::FetchClient::getInstance().fetchUserInfoByUserId(id_);
    if (userInfo->getRoomId() == "0")
    {
        throw bilibili::api::FetchClientException("User Not Found");
    }

    auto roomId = userInfo->getRoomId();

    auto roomInfo =
        bilibili::api::FetchClient::getInstance().fetchRoomInfoByRoomId(roomId);

    return roomInfo->getLiveStatus();
}
