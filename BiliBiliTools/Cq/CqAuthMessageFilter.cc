//
// Created by diam on 24-1-20.
//

#include "CqAuthMessageFilter.h"

namespace cq
{
CqAuthMessageFilter::CqAuthMessageFilter()
{
    const auto authArray = drogon::app().getCustomConfig()["cqhttp"]["auth"];
    for (const Json::Value &id : authArray)
    {
        authedIdList.push_back(id.asString());
    }
}

bool CqAuthMessageFilter::doFilter(const CqMessageData &data)
{
    if (!data.second["sender"]["user_id"].isNumeric() ||
        !data.second["message"].isString())
    {
        return false;
    }
    auto senderId = data.second["sender"]["user_id"].asString();

    const auto findResult =
        std::find(authedIdList.cbegin(), authedIdList.cend(), senderId);
    if (findResult != authedIdList.cend())
    {
        return true;
    }

    return false;
}
}  // namespace cq
