//
// Created by diam on 24-1-20.
//

#pragma once

#include "CqMessageFilter.h"
#include "Common.h"

namespace cq
{

class CqPrivateChatMessageFilter : public virtual CqMessageFilter
{
  public:
    CqPrivateChatMessageFilter() = default;

    bool doFilter(const cq::CqMessageData &) override;

    ~CqPrivateChatMessageFilter() override = default;

    SINGLETON_INTERFACE(CqPrivateChatMessageFilter);
};

}  // namespace cq
