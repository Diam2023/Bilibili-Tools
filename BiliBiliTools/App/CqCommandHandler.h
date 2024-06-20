//
// Created by diam on 24-1-20.
//

#ifndef IOTSERVER_CQCOMMANDHANDLER_H
#define IOTSERVER_CQCOMMANDHANDLER_H

#include <json/value.h>
#include <string>
#include <tuple>

#include "Common.h"
#include "CqMessageHandler.h"

#include <queue>

#include <regex>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <unordered_map>

namespace cq
{

enum class ChatMessageType
{
    Private,
    Group
};

// botId senderId groupId msg senderType

using ChatBotIdType = std::string;
using ChatSenderIdType = std::string;
using ChatGroupIdType = std::string;
using ChatMessageDataType = std::string;

using CqChatMessageData = std::tuple<ChatBotIdType,
                                     ChatSenderIdType,
                                     ChatGroupIdType,
                                     ChatMessageDataType,
                                     ChatMessageType>;

class CqCommandHandler
{
  private:
    // Command Queue
    std::queue<CqChatMessageData> commandQueue;
    std::mutex commandQueueMutex;
    std::condition_variable commandQueueCondVar;

    std::thread commandWorkThread;
    Json::Value hintMessages;

    // 指令列表
    std::unordered_map<std::string, std::regex> commandRegex;

    // 命令缓存前缀
    std::string commandPrefix;

    // 动态帮助信息
    std::string helpMessage;

    // 详细帮助信息
    std::unordered_map<std::string, std::string> helpDetailMessage;

  public:
    CqCommandHandler();

    void matchCommand(const CqChatMessageData &data);

    /**
     * @brief 处理命令
     */
    [[noreturn]] void handleCommand();

    void pushCommand(const CqChatMessageData &data);

    /**
     * @brief 初始化 添加Regex
     *
     */
    void init();

    SINGLETON_INTERFACE(CqCommandHandler);

    /**
     * @brief 将ChatMessage转化为Cq可发送的信息
     *
     * @param data
     * @return CqMessageData
     */
    static CqMessageData MakeCqMesageData(const CqChatMessageData &data);

    /**
     * @brief 处理帮助的返回信息
     *
     * @param data
     * @param subHelp
     */
    void helpHandler(const CqChatMessageData &data, const std::string &subHelp);

    /**
     * @brief 输出版本信息
     *
     * @param data
     */
    void versionHandler(const CqChatMessageData &data);

    /**
     * @brief 更新缓存
     * 
     * @param data 
     */
    void renewHandler(const CqChatMessageData &data);

    /**
     * @brief 输出所有订阅信息
     *
     * @param data
     */
    void listsHandler(const CqChatMessageData &data);

    /**
     * @brief 取消订阅房间
     *
     * @param data 消息
     * @param rid RoomId
     */
    void unsubscribeHanlder(const CqChatMessageData &data,
                            const std::string &rid);

    /**
     * @brief 订阅房间
     *
     * @param data 消息
     * @param rid RoomId
     * @param timer DelayTime
     */
    void subscriberHanlder(const CqChatMessageData &data,
                           const std::string &rid,
                           const std::string &timer);

    /**
     * @brief 订阅用户房间
     *
     * @param data 消息
     * @param uid UserId
     * @param timer DelayTime
     */
    void subscribeHanlder(const CqChatMessageData &data,
                          const std::string &uid,
                          const std::string &timer);
};

}  // namespace cq

#endif  // IOTSERVER_CQCOMMANDHANDLER_H
