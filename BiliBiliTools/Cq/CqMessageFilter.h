//
// Created by diam on 24-1-20.
//

#pragma once
#include <string>
#include <utility>
#include <json/json.h>

namespace cq
{

// string is bot id value is json data from bot received
using CqMessageData = std::pair<std::string, Json::Value>;

class CqMessageFilter
{
  public:
    CqMessageFilter() = default;

    // TODO Restructure To ()
    virtual bool doFilter(const CqMessageData &) = 0;

    bool operator()(const CqMessageData &d)
    {
        return doFilter(d);
    }

    virtual ~CqMessageFilter() = default;
};

}  // namespace cq
