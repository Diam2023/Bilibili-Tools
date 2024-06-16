//
// Created by diam on 24-1-20.
//

#ifndef IOTSERVER_CQCOMMANDHANDLER_H
#define IOTSERVER_CQCOMMANDHANDLER_H

#include <string>
#include <tuple>

#include "Common.h"

#include <queue>

#include <regex>
#include <mutex>
#include <condition_variable>
#include <thread>

namespace cq
{

// botId senderId msg
using CqCommandData = std::tuple<std::string, std::string, std::string>;

class CqCommand
{
  private:
    // Command Queue
    std::queue<CqCommandData> commandQueue;
    std::mutex commandQueueMutex;
    std::condition_variable commandQueueCondVar;

    std::thread commandWorkThread;

    // 登入 un pwd
    std::regex loginRegex;
    // 登出
    std::regex logoutRegex;

    // 添加设备 sn
    std::regex addDeviceRegex;
    // 删除设备 sn
    std::regex removeDeviceRegex;
    // 查看所有设备
    std::regex listDeviceRegex;

    // 订阅设备消息 sn
    std::regex subscribeDeviceMessageRegex;
    // 取消订阅设备消息 sn
    std::regex unsubscribeDeviceMessageRegex;
    // 查看所有订阅消息
    std::regex listSubscribeDeviceMessageRegex;

    // 查看设备别名
    std::regex listDeviceAliasRegex;
    // 添加别名 sn name
    std::regex addDeviceAliasRegex;
    // 删除别名 name
    std::regex removeDeviceAliasRegex;

    std::regex listActionRegex;
    // 定义操作 {操作名} {设备名/别名} {subtopic} {JSON数据}
    std::regex makeActionRegex;
    // 删除操作
    std::regex deleteActionRegex;
    // 以Qos0直接发布消息
    std::regex launchMessageRegex;

    // name打开
    // name关闭
    //         std::regex actionCallerRegex

    // show help message
    std::regex helpRegex;

  public:
    CqCommand();

    void matchCommand(const CqCommandData &data);

    [[noreturn]] void handlerCommand();

    void pushCommand(const CqCommandData &data);

    SINGLETON_INTERFACE(CqCommand);
};

}  // namespace cq

#endif  // IOTSERVER_CQCOMMANDHANDLER_H
