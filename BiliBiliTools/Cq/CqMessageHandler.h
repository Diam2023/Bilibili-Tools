/**
 * @file CqMessageHandler.h
 * @author Diam (monoluths-uni@outlook.com)
 * @brief
 * @version 0.1-alpha
 * @date 2024-06-16
 *
 * @copyright Copyright (c) 2024 Diam
 *
 */

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
