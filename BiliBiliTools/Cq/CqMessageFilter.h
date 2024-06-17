//
// Created by diam on 24-1-20.
//

#pragma once

#include "CqMessageHandler.h"

namespace cq
{

class CqMessageFilter
{
  public:
    CqMessageFilter() = default;

    virtual bool doFilter(const CqMessageData &) = 0;

    virtual ~CqMessageFilter(){};
};

}  // namespace cq
