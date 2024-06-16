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

#include "CqCommand.h"

#include "CqMessageManager.h"
#include <drogon/drogon.h>
#include <json/value.h>
#include <regex>
#include <sstream>

namespace cq
{

void CqCommand::pushCommand(const CqCommandData &command)
{
    std::lock_guard<std::mutex> lg(commandQueueMutex);
    commandQueue.push(command);
    commandQueueCondVar.notify_one();
}

// TODO Call In Main
void CqCommand::init()
{
    // Gen Command And Prefix
    const auto config = drogon::app().getCustomConfig()["cqhttp"];
    commandPrefix = config["command_prefix"].asString();
    std::stringstream helpMessageStream;

    const auto commandArray = config["command"];
    for (auto &commandItem : commandArray)
    {
        // 动态生成regex缓存
        commandRegex[commandItem["name"].asString()] =
            std::regex(commandItem["match_rule"].asString());

        // 动态生成帮助信息
        // TODO 优化 写入到文件内
        helpMessageStream.clear();
        helpMessageStream << commandItem["command"].asString() << std::endl;
        helpMessageStream << commandItem["short_help"].asString();
        if (commandItem != *commandArray.end())
        {
            helpMessageStream << std::endl;
        }

        std::stringstream helpDetailMessageStream;
        if (!commandItem["full_help"].empty())
        {
            for (auto &commandFullMessage : commandItem["full_help"]["message"])
            {
                helpDetailMessageStream << commandFullMessage.asString();
                if (commandFullMessage != *commandItem.end())
                {
                    helpDetailMessageStream << std::endl;
                }
            }
        }

        // 生成详细帮助信息
        helpDetailMessage[commandItem["name"].asString()] =
            helpDetailMessageStream.str();
    }
    helpMessage = helpMessageStream.str();
}

[[noreturn]] void CqCommand::handlerCommand()
{
    using namespace std::chrono_literals;

    while (true)
    {
        CqCommandData data;
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

void CqCommand::matchCommand(const CqCommandData &data)
{
    auto botId = std::get<0>(data);
    auto senderId = std::get<1>(data);
    auto message = std::get<2>(data);

    do
    {
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

        cq::CqMessageManager::getInstance().messageOut(
            botId,
            senderId,
            "Non Matched Command Please use help display command list");

    } while (false);
}

CqCommand::CqCommand() : commandWorkThread(&CqCommand::handlerCommand, this)
{
}
}  // namespace cq
