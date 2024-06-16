//
// Created by diam on 24-1-20.
//

#include "CqAuthedMessageHandler.h"

#include <drogon/drogon.h>
#include "CqCommand.h"

namespace cq
{
void CqAuthedMessageHandler::handler(const CqMessageData &data)
{
    std::string senderId;
    std::string receivedMessage;

    do
    {
        if (!data.second["sender"]["user_id"].isNumeric() ||
            !data.second["message"].isString())
        {
            break;
        }

        senderId = data.second["sender"]["user_id"].asString();
        receivedMessage = data.second["message"].asString();

        if (senderId.empty() || receivedMessage.empty())
        {
            break;
        }

        // TODO Call login or another here
        //            LOG_INFO << "private message received: " << data.first <<
        //            " sender: " << senderId << " msg: "
        //                     << receivedMessage;

        auto commandData =
            std::make_tuple(data.first, senderId, receivedMessage);
        CqCommand::getInstance().pushCommand(commandData);

        //            switch () {

        //            }

    } while (false);

    // Get Private id
}
}  // namespace cq
