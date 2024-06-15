#ifndef BILIBILIROOMINFO_H
#define BILIBILIROOMINFO_H

#include <string>

namespace bilibili::model
{

class UserInfo;

/**
 * Live Room Info
 */
class RoomInfo
{
  public:
    enum class LiveStatus
    {
        OffLine,  // 未直播
        OnLine,   // 直播中
        Carousel  // 轮播
    };

  public:
    RoomInfo();
    RoomInfo(const std::string &id_,
             const std::string &userId_,
             const LiveStatus &liveStatus_,
             const std::string &areaName_,
             const std::string &title_,
             const std::string &frameImage_);
    RoomInfo(const RoomInfo &roomInfo_);

    virtual ~RoomInfo();

    std::string getId() const
    {
        return id;
    }

    void setId(const std::string &id_)
    {
        id = id_;
    }

    std::string getUserId() const
    {
        return userId;
    }

    void setUserId(const std::string &userId_)
    {
        userId = userId_;
    }

    LiveStatus getLiveStatus() const
    {
        return liveStatus;
    }

    void setLiveStatus(const LiveStatus &liveStatus_)
    {
        liveStatus = liveStatus_;
    }

    std::string getAreaName() const
    {
        return areaName;
    }

    void setAreaName(const std::string &areaName_)
    {
        areaName = areaName_;
    }

    std::string getTitle() const
    {
        return title;
    }

    void setTitle(const std::string &title_)
    {
        title = title_;
    }

    std::string getFrameImage() const
    {
        return frameImage;
    }

    void setFrameImage(const std::string &frameImage_)
    {
        frameImage = frameImage_;
    }

  private:
    std::string id;

    // 争议 用户ID
    std::string userId;

    LiveStatus liveStatus;
    std::string areaName;  // 分区名
    std::string title;     // 标题

    std::string frameImage;  // 帧截图
};

}  // namespace bilibili::model

#endif  // BILIBILIROOMINFO_H
