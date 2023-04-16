#pragma once

#include <MqttController.hpp>

class MqttCallback : MqttListener {
    public:
    MqttCallback(MqttController& mqtt_controller, const char* topic, void (*mqttCallback)(const char*, const char*))
        :MqttListener(mqtt_controller, topic), mqttCallback(mqttCallback) {};

    virtual bool presentMessage(const char *topic,const char *payload);

    protected:
    void (*mqttCallback)(const char*, const char*);
};