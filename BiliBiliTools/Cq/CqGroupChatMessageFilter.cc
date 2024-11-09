/**
 * @file CqGroupChatMessageFilter.cc
 * @author diam (monoliths-uni@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-06-13
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "CqGroupChatMessageFilter.h"

bool cq::CqGroupChatMessageFilter::doFilter(const cq::CqMessageData &data)
{
    auto messageDataJsonData = (*data.second)["message_type"];
    return messageDataJsonData.asString() == "group";
}
