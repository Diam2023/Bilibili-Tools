#include "BiliBiliRoomInfo.h"

namespace bilibili::model
{

RoomInfo::RoomInfo()
{
}

RoomInfo::RoomInfo(const std::string &id_,
                   const std::string &userId_,
                   const LiveStatus &liveStatus_,
                   const std::string &areaName_,
                   const std::string &title_,
                   const std::string &frameImage_)
    : id(id_),
      userId(userId_),
      liveStatus(liveStatus_),
      areaName(areaName_),
      title(title_),
      frameImage(frameImage_)
{
}

RoomInfo::RoomInfo(const RoomInfo &roomInfo_)
    : id(roomInfo_.id),
      userId(roomInfo_.userId),
      liveStatus(roomInfo_.liveStatus),
      areaName(roomInfo_.areaName),
      title(roomInfo_.title),
      frameImage(roomInfo_.frameImage)
{
}

RoomInfo::~RoomInfo()
{
}

}  // namespace bilibili::model
