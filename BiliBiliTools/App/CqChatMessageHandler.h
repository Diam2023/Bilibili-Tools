/**
 * @file CqChatMessaeHandler.h
 * @author Diam (monoluths-uni@outlook.com)
 * @brief
 * @version 0.1-alpha
 * @date 2024-06-17
 *
 * @copyright Copyright (c) 2024 Diam
 *
 */
#ifndef CQCHATMESSAGEHANDLER_H
#define CQCHATMESSAGEHANDLER_H

#include "Common.h"
#include "CqMessageHandler.h"

namespace cq
{

class ChatMessageHandler : public CqMessageHandler
{
  public:
    ChatMessageHandler() = default;
    ~ChatMessageHandler() = default;

    /**
     * @brief 消息处理 将消息打包为CqChatMessageData类型
     *
     * @param data
     */
    void handler(const CqMessageData &data) override;

    SINGLETON_INTERFACE(ChatMessageHandler);

  private:
};

}  // namespace cq

#endif  // CQCHATMESSAGEHANDLER_H
