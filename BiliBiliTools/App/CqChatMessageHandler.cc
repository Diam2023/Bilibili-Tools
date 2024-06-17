#include "CqChatMessageHandler.h"

#include <drogon/drogon.h>
#include <trantor/utils/Logger.h>

#include "CqCommandHandler.h"
#include "CqGroupChatMessageFilter.h"

void cq::ChatMessageHandler::handler(const CqMessageData &data)
{
    // 发送者
    ChatSenderIdType senderId;
    // 哪一个群聊发的信息
    ChatGroupIdType groupId;
    // 接收到的消息
    ChatMessageDataType receivedMessage;

    do
    {
        ChatMessageType type;
        if (CqGroupChatMessageFilter::getInstance().doFilter(data))
        {
            type = ChatMessageType::Group;
            if (!data.second["sender"]["user_id"].isNumeric() ||
                !data.second["group_id"].isNumeric() ||
                !data.second["message"].isString())
            {
                break;
            }

            senderId = data.second["sender"]["user_id"].asString();
            groupId = data.second["group_id"].asString();
            receivedMessage = data.second["message"].asString();

            if (senderId.empty() || groupId.empty() || receivedMessage.empty())
            {
                break;
            }
        }
        else
        {
            type = ChatMessageType::Private;
            if (!data.second["sender"]["user_id"].isNumeric() ||
                !data.second["message"].isString())
            {
                break;
            }

            senderId = data.second["sender"]["user_id"].asString();
            receivedMessage = data.second["message"].asString();

            if (senderId.empty() || receivedMessage.empty())
            {
                break;
            }
        }

        // data.first 为bot qq
        // 推送到处理线程
        auto commandData = CqChatMessageData(
            data.first, senderId, groupId, receivedMessage, type);
        cq::CqCommandHandler::getInstance().pushCommand(commandData);

    } while (false);

    // LOG_DEBUG << "UnHandler Message: " << data.second.toStyledString();
}
