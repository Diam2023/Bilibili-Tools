/**
 * @file CqAuthedMessageHandler.h
 * @author Diam (monoluths-uni@outlook.com)
 * @brief
 * @version 0.1-alpha
 * @date 2024-06-16
 *
 * @copyright Copyright (c) 2024 Diam
 *
 */
#ifndef CQAUTHEDMESSAGEHANDLER_H
#define CQAUTHEDMESSAGEHANDLER_H

#include "CqMessageHandler.h"

namespace cq
{

class CqAuthedMessageHandler : public virtual CqMessageHandler
{
  public:
    CqAuthedMessageHandler() = default;

    void handler(const cq::CqMessageData &) override;

    ~CqAuthedMessageHandler() override = default;

    static CqAuthedMessageHandler &getInstance()
    {
        static CqAuthedMessageHandler messageHandler;
        return messageHandler;
    }
};

}  // namespace cq

#endif  // CQAUTHEDMESSAGEHANDLER_H
