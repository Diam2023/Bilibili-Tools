/**
 * @file CqAdminMessageFilter.h
 * @author Diam (monoluths-uni@outlook.com)
 * @brief
 * @version 0.1-alpha
 * @date 2024-06-16
 *
 * @copyright Copyright (c) 2024 Diam
 *
 */

#ifndef CQADMINMESSAGEFILTER_H
#define CQADMINMESSAGEFILTER_H

#include <drogon/drogon.h>

#include "CqMessageFilter.h"

#include "Common.h"

namespace cq
{

class CqAdminMessageFilter : public virtual CqMessageFilter
{
  public:
    CqAdminMessageFilter();

    bool doFilter(const cq::CqMessageData &) override;

    ~CqAdminMessageFilter() override = default;

    SINGLETON_INTERFACE(CqAdminMessageFilter);
};

}  // namespace cq

#endif  // CQADMINMESSAGEFILTER_H
