#include "BiliBiliUserInfo.h"

namespace bilibili::model
{

UserInfo::UserInfo()
{
}

UserInfo::UserInfo(const std::string &id_,
                   const std::string &roomId_,
                   const std::string &name_,
                   const std::string &headerUrl_,
                   uint64_t level_)
    : id(id_),
      roomId(roomId_),
      name(name_),
      headerUrl(headerUrl_),
      level(level_)
{
}

UserInfo::UserInfo(const UserInfo &userInfo_)
    : id(userInfo_.id),
      name(userInfo_.name),
      headerUrl(userInfo_.headerUrl),
      level(userInfo_.level)
{
}

UserInfo::~UserInfo()
{
}

}  // namespace bilibili::model
