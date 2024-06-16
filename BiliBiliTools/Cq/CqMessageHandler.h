//
// Created by diam on 24-1-20.
//

#pragma once

#include <json/json.h>
#include <string>
#include <functional>

namespace cq
{

// string is bot id value is json data from bot received
using CqMessageData = std::pair<std::string, Json::Value>;

using CqMessageHandlerType = std::function<void(const CqMessageData &)>;

class CqMessageHandler
{
  public:
    CqMessageHandler() = default;

    virtual void handler(const CqMessageData &) = 0;

    void operator()(const CqMessageData &d)
    {
        handler(d);
    }

    virtual ~CqMessageHandler(){};
};

}  // namespace cq
