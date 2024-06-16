//
// Created by diam on 24-1-19.
//

#pragma once

#include "MqttMessageHandler.h"

namespace cq
{
class CqMqttDeviceMessageHandler : public mqtt::MqttMessageHandler
{
  public:
    CqMqttDeviceMessageHandler();

    /**
     * Message Handler
     * @param data
     */
    void handler(const mqtt::MqttData &data) override;

    /**
     * To Format Dis play Json
     * @param data
     * @param deviceName device name
     * @return
     */
    static std::string convertVisual(const mqtt::MqttData &data,
                                     const std::string &deviceName);

    static CqMqttDeviceMessageHandler &getInstance()
    {
        static CqMqttDeviceMessageHandler cqMqttMessageHandler;
        return cqMqttMessageHandler;
    }
};
}  // namespace cq
