//
// Created by diam on 24-1-20.
//

#pragma once

#include "CqMessageFilter.h"
#include "Common.h"

namespace cq
{

class CqGroupChatMessageFilter : public virtual CqMessageFilter
{
  public:
    CqGroupChatMessageFilter() = default;

    bool doFilter(const cq::CqMessageData &) override;

    ~CqGroupChatMessageFilter() override = default;

    SINGLETON_INTERFACE(CqGroupChatMessageFilter);
};

}  // namespace cq
