#pragma once

#include <string>
#include <cstdint>

namespace bilibili::model
{

/**
 * @brief b站用户信息
 */
class UserInfo
{
  public:
    UserInfo();
    UserInfo(const std::string &id_,
             const std::string &roomId_,
             const std::string &name_,
             const std::string &headerUrl_,
             uint64_t level_);
    UserInfo(const UserInfo &userInfo_);

    virtual ~UserInfo();

    std::string getId() const
    {
        return id;
    }

    void setId(const std::string &id_)
    {
        id = id_;
    }

    std::string getName() const
    {
        return name;
    }

    void setName(const std::string &name_)
    {
        name = name_;
    }

    std::string getHeaderUrl() const
    {
        return headerUrl;
    }

    void setHeaderUrl(const std::string &headerUrl_)
    {
        headerUrl = headerUrl_;
    }

    uint64_t getLevel() const
    {
        return level;
    }

    void setLevel(const uint64_t &level_)
    {
        level = level_;
    }

    std::string getRoomId() const
    {
        return roomId;
    }

    void setRoomId(const std::string &roomId_)
    {
        roomId = roomId_;
    }

  private:
    std::string id;
    std::string roomId;
    std::string name;
    std::string headerUrl;
    uint64_t level;
};

}  // namespace bilibili::model

