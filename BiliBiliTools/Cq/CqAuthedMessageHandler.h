//
// Created by diam on 24-1-20.
//

#ifndef IOTSERVER_CQAUTHEDMESSAGEHANDLER_H
#define IOTSERVER_CQAUTHEDMESSAGEHANDLER_H

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

#endif  // IOTSERVER_CQAUTHEDMESSAGEHANDLER_H
