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
#include <unordered_map>

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

    // 指令列表
    std::unordered_map<std::string, std::regex> commandRegex;

    // 命令缓存前缀
    std::string commandPrefix;

    // 动态帮助信息
    std::string helpMessage;

    // 详细帮助信息
    std::unordered_map<std::string, std::string> helpDetailMessage;

  public:
    CqCommand();

    void matchCommand(const CqCommandData &data);

    [[noreturn]] void handlerCommand();

    void pushCommand(const CqCommandData &data);

    /**
     * @brief 初始化 添加Regex
     *
     */
    void init();

    SINGLETON_INTERFACE(CqCommand);
};

}  // namespace cq

#endif  // IOTSERVER_CQCOMMANDHANDLER_H
