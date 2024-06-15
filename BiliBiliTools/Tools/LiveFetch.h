#ifndef LIVEFETCH_H
#define LIVEFETCH_H

#include <string>

#include "BiliBiliRoomInfo.h"

namespace bilibili::tools
{

/**
 * @brief 简化获取房间直播状态
 *
 * @return id_ 用户id
 */
bilibili::model::RoomInfo::LiveStatus FetchRoomStatusByUserId(
    const std::string &id_);

};  // namespace bilibili::tools

#endif // LIVEFETCH_H
