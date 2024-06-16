//
// Created by diam on 24-1-20.
//

#include "CqPrivateChatMessageFilter.h"

bool cq::CqPrivateChatMessageFilter::doFilter(const cq::CqMessageData &data)
{
    auto messageDataJsonData = data.second["message_type"];
    return messageDataJsonData.asString() == "private";
}
