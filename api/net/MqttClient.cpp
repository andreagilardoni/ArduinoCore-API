#include "MqttClient.h"

//TODO define constant value for pimpl not instantiated
int MqttClient::connect(IPAddress ip, uint16_t port) {
    return impl != nullptr? impl->connect(ip, port) : -1;
}

int MqttClient::connect(const char *host, uint16_t port) {
    return impl != nullptr? impl->connect(host, port) : -1;
}

void MqttClient::disconnect() {
    if(impl != nullptr) {
        return impl->disconnect();
    }
}

uint8_t MqttClient::connected() {
    return impl != nullptr? impl->connected() : 0;
}

MqttClient::operator bool() {
    // FIXME
    return impl != nullptr? impl->operator bool() : false;
}

error_t MqttClient::subscribe(Topic t, MqttQos qos) {
    return impl != nullptr? impl->subscribe(t, qos) : -1;
}

error_t MqttClient::publish(Topic t, uint8_t payload[], size_t size, MqttQos qos) {
    return impl != nullptr? impl->publish(t, payload, size, qos) : -1;
}

error_t MqttClient::unsubscribe(Topic t) {
    return impl != nullptr? impl->unsubscribe(t) : -1;
}

void MqttClient::poll() {
    if(impl != nullptr) {
        return impl->poll();
    }
}

error_t MqttClient::ping() {
    return impl != nullptr? impl->ping() : -1;
}

