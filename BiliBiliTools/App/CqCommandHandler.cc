/**
 * @file CqCommand.cc
 * @author Diam (monoliths-uni@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-06-16
 *
 * @copyright Copyright (c) 2024 Diam
 *
 */

#include "CqCommandHandler.h"

#include <drogon/drogon.h>
#include <drogon/orm/Mapper.h>
#include <json/value.h>
#include <trantor/utils/Logger.h>
#include <cstdio>
#include <cstring>
#include <exception>
#include <regex>
#include <sstream>
#include <string>
#include "CqMessageManager.h"

namespace cq
{

static char tempString[512];

void CqCommandHandler::pushCommand(const CqChatMessageData &command)
{
    std::lock_guard<std::mutex> lg(commandQueueMutex);
    commandQueue.push(command);
    commandQueueCondVar.notify_one();
}

void CqCommandHandler::init()
{
    // Gen Command And Prefix
    const auto config = drogon::app().getCustomConfig()["cqhttp"];
    commandPrefix = config["command_prefix"].asString();
    std::stringstream helpMessageStream;

    const auto commandArray = config["command"];
    for (auto &commandItem : commandArray)
    {
        {
            // 动态生成regex缓存
            commandRegex[commandItem["name"].asString()] = std::regex(
                commandPrefix + commandItem["match_rule"].asString());
        }

        {
            // 动态生成帮助信息
            helpMessageStream.clear();
            helpMessageStream << commandPrefix
                              << commandItem["command"].asString() << std::endl;
            helpMessageStream << config["short_help_prefix"].asString()
                              << commandItem["short_help"].asString();
            helpMessageStream << std::endl;
            helpMessageStream << std::endl;
        }

        {
            // 生成详细帮助信息
            std::stringstream helpDetailMessageStream;
            if (!commandItem["full_help"].empty())
            {
                for (auto &commandFullMessage :
                     commandItem["full_help"]["message"])
                {
                    helpDetailMessageStream << commandFullMessage.asString();
                    helpDetailMessageStream << std::endl;
                }
            }
            if (!helpDetailMessageStream.str().empty())
            {
                if (!commandPrefix.empty())
                {
                    helpDetailMessageStream << "命令前缀 [" << commandPrefix
                                            << "]";
                }
            }
            helpDetailMessage[commandItem["name"].asString()] =
                helpDetailMessageStream.str();
        }
    }

    {
        // Version Info
        helpMessageStream << "Powered by " << config["bot_name"].asString();
        helpMessage = helpMessageStream.str();
    }
}

CqMessageData CqCommandHandler::MakeCqMesageData(const CqChatMessageData &data)
{
    // https://docs.go-cqhttp.org/api/#%E5%8F%91%E9%80%81%E7%A7%81%E8%81%8A%E6%B6%88%E6%81%AF
    // TODO Add More message Support
    ChatBotIdType botId = std::get<0>(data);
    // 发送到的人
    ChatSenderIdType senderId = std::get<1>(data);
    // 发送到的群组
    ChatGroupIdType groupId = std::get<2>(data);
    ChatMessageDataType message = std::get<3>(data);
    ChatMessageType messageType = std::get<4>(data);
    int64_t senderIdNum = 0;
    int64_t groupIdNum = 0;
    Json::Value jsonData;

    if (!senderId.empty())
    {
        senderIdNum = std::stoll(senderId);
    }
    if (!groupId.empty())
    {
        groupIdNum = std::stoll(groupId);
    }
    jsonData["action"] = "send_msg";

    if (messageType == ChatMessageType::Private)
    {
        jsonData["params"]["message_type"] = "private";
    }
    else
    {
        jsonData["params"]["message_type"] = "group";
    }
    jsonData["params"]["user_id"] = senderIdNum;
    jsonData["params"]["group_id"] = groupIdNum;
    jsonData["params"]["message"] = message;
    jsonData["params"]["auto_escape"] = true;
    return {botId, jsonData};
}

void CqCommandHandler::helpHandler(const CqChatMessageData &data,
                                   const std::string &subHelp)
{
    auto outData = data;
    if (subHelp.empty())
    {
        // 输出短帮助信息
        std::get<3>(outData) = helpMessage;
    }
    else
    {
        try
        {
            if (helpDetailMessage[subHelp].empty())
            {
                // TODO 优化新异常类
                throw std::exception();
            }
            // 输出长帮助信息
            std::get<3>(outData) = helpDetailMessage[subHelp];
        }
        catch (const std::exception &e)
        {
            // USE C++ 20 To Replease
            memset(tempString, 0, sizeof(tempString));
            sprintf(tempString, hintMessages[10].asCString(), subHelp.c_str());
            std::get<3>(outData) = std::string(tempString);
        }
    }
    CqMessageManager::getInstance().messageOut(MakeCqMesageData(outData));
}

void CqCommandHandler::versionHandler(const CqChatMessageData &data)
{
    auto outData = data;
    std::get<3>(outData) = "// TODO 待完善";
    CqMessageManager::getInstance().messageOut(MakeCqMesageData(outData));
}

void CqCommandHandler::listsHandler(const CqChatMessageData &data)
{
    
}

void CqCommandHandler::unsubscribeHanlder(const CqChatMessageData &data,
                                          const std::string &rid)
{
}

void CqCommandHandler::subscriberHanlder(const CqChatMessageData &data,
                                         const std::string &rid,
                                         const std::string &timer)
{
}

void CqCommandHandler::subscribeHanlder(const CqChatMessageData &data,
                                        const std::string &uid,
                                        const std::string &timer)
{
    // drogon::orm::Mapper<typename T>
}

[[noreturn]] void CqCommandHandler::handleCommand()
{
    using namespace std::chrono_literals;

    while (true)
    {
        CqChatMessageData data;
        {
            std::unique_lock<std::mutex> ul(commandQueueMutex);
            commandQueueCondVar.wait(ul, [this]() {
                return !commandQueue.empty();
            });
            data = commandQueue.front();
            commandQueue.pop();
        }  // Release Lock
        matchCommand(data);

        std::this_thread::sleep_for(1ms);
    }
}

void CqCommandHandler::matchCommand(const CqChatMessageData &data)
{
    // ChatBotIdType botId = std::get<0>(data);
    // ChatSenderIdType senderId = std::get<1>(data);
    // ChatGroupIdType groupId = std::get<2>(data);
    ChatMessageDataType message = std::get<3>(data);
    // ChatMessageType messageType = std::get<4>(data);

    // 输出缓存
    CqChatMessageData tempSendData;

    for (auto const &[commandName, commandRule] : commandRegex)
    {
        bool matchResult = std::regex_match(message, commandRule);
        if (matchResult)
        {
            std::sregex_iterator argIter(message.cbegin(),
                                         message.cend(),
                                         commandRule);
            std::string matchCommandName = argIter->str(1);
            if (matchCommandName == "help")
            {
                helpHandler(data, argIter->str(2));
            }
            else if (matchCommandName == "version")
            {
                versionHandler(data);
            }
            else if (matchCommandName == "lists")
            {
                listsHandler(data);
            }
            else if (matchCommandName == "unsubscribe")
            {
                unsubscribeHanlder(data, argIter->str(2));
            }
            else if (matchCommandName == "subscriber")
            {
                subscriberHanlder(data, argIter->str(2), argIter->str(3));
            }
            else if (matchCommandName == "subscribe")
            {
                subscribeHanlder(data, argIter->str(2), argIter->str(3));
            }
        }
    }
    // std::string token;
    // // Check If Non Login
    // try
    // {
    //     token = cq::CqUserApi::getCqLoginInfo(senderId).get();
    // }
    // catch (const std::exception &e)
    // {
    //     // Non Login
    // }
    // if (!token.empty())
    // {
    //     try
    //     {
    //         auto res = api::ActionApi::matchAction(token, message).get();
    //         mqtt::MqttMessagePublisher::getInstance().sendMessage(res);
    //         cq::CqMessageManager::getInstance().messageOut(botId,
    //                                                        senderId,
    //                                                        "Action
    //                                                        pushed");
    //         break;
    //     }
    //     catch (const std::exception &e)
    //     {
    //         // Non Match
    //     }
    // }

    // cq::CqMessageManager::getInstance().messageOut(
    //     botId,
    //     senderId,
    //     "Non Matched Command Please use help display command list");
}

CqCommandHandler::CqCommandHandler()
    : commandWorkThread(&CqCommandHandler::handleCommand, this)
{
    hintMessages = drogon::app().getCustomConfig()["cqhttp"]["hint_messages"];
}
}  // namespace cq
