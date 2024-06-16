//
// Created by diam on 24-1-20.
//

#ifndef IOTSERVER_CQAUTHMESSAGEFILTER_H
#define IOTSERVER_CQAUTHMESSAGEFILTER_H

#include <drogon/drogon.h>

#include "CqMessageFilter.h"

#include "Common.h"

namespace cq
{

class CqAuthMessageFilter : public virtual CqMessageFilter
{
    std::vector<std::string> authedIdList;

  public:
    CqAuthMessageFilter();

    bool doFilter(const cq::CqMessageData &) override;

    ~CqAuthMessageFilter() override = default;

    SINGLETON_INTERFACE(CqAuthMessageFilter);
};

}  // namespace cq

#endif  // IOTSERVER_CQAUTHMESSAGEFILTER_H
