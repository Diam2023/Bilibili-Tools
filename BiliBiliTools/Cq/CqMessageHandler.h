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

#include <CqMessageFilter.h>
#include <functional>

namespace cq
{
class CqMessageFilter;

using CqMessageHandlerType = std::function<void(const CqMessageData &)>;

class CqMessageHandler
{
  public:
    CqMessageHandler() = default;

    /**
     * Regist a Filter
     * @param filterPtr filter
     */
    void registerFilter(const std::shared_ptr<CqMessageFilter> &filterPtr)
    {
        // Copy It
        messageFilterList.push_back(filterPtr);
    }

    void operator()(const CqMessageData &d)
    {
        bool lastCheckResult = true;
        for (const auto &messageFilter : messageFilterList)
        {
            // Call Filter
            lastCheckResult = (*messageFilter)(d);
            if (!lastCheckResult)
            {
                break;
            }
        }

        if (lastCheckResult)
        {
            handler(d);
        }
    }

    virtual ~CqMessageHandler() = default;

  protected:
    virtual void handler(const CqMessageData &) = 0;

  private:
    std::vector<std::shared_ptr<CqMessageFilter>> messageFilterList;
};

}  // namespace cq
