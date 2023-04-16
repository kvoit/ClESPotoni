#include <MqttCallback.hpp>

bool MqttCallback::presentMessage(const char *topic,const char *payload) {
    if (!strcmp(this->getMQTTTopic(), topic)) {
        if (mqttCallback != nullptr) {
            mqttCallback(topic, payload);
            return true;
        }
    }
    return false;
}