/**
 * @file CqPrivateChatMessageFilter.h
 * @author Diam (monoluths-uni@outlook.com)
 * @brief
 * @version 0.1-alpha
 * @date 2024-06-16
 *
 * @copyright Copyright (c) 2024 Diam
 *
 */

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
