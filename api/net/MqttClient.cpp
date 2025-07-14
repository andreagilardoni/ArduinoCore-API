#include "MqttClient.h"
#include <Arduino.h>

// std::function<std::unique_ptr<MqttClientInterface>()>* MqttClient::_factory = nullptr;
// std::function<std::unique_ptr<MqttClientInterface>()> MqttClient::_factory;
static std::function<std::unique_ptr<MqttClientInterface>()>* _factory=nullptr;


MqttClient::MqttClient()
: impl(nullptr) {

}

//TODO define constant value for pimpl not instantiated
int MqttClient::connect(IPAddress ip, uint16_t port) {
    checkInstance();
    return impl != nullptr? impl->connect(ip, port) : -1;
}

int MqttClient::connect(const char *host, uint16_t port) {
    checkInstance();
    return impl != nullptr? impl->connect(host, port) : -1;
}

void MqttClient::disconnect() {
    checkInstance();
    if(impl != nullptr) {
        impl->disconnect();
    }
    // TODO we may delete the internal client
}

uint8_t MqttClient::connected() {
    checkInstance();
    return impl != nullptr? impl->connected() : 0;
}

MqttClient::operator bool() {
    checkInstance();
    // FIXME check operator bool call works
    return impl != nullptr? impl->operator bool() : false;
}

error_t MqttClient::subscribe(Topic t, MqttQos qos) {
    checkInstance();
    return impl != nullptr? impl->subscribe(t, qos) : -1;
}

error_t MqttClient::publish(Topic t, uint8_t payload[], size_t size, MqttQos qos) {
    checkInstance();
    return impl != nullptr? impl->publish(t, payload, size, qos) : -1;
}

error_t MqttClient::unsubscribe(Topic t) {
    checkInstance();
    return impl != nullptr? impl->unsubscribe(t) : -1;
}

void MqttClient::poll() {
    checkInstance();
    if(impl != nullptr) {
        return impl->poll();
    }
}

error_t MqttClient::ping() {
    checkInstance();
    return impl != nullptr? impl->ping() : -1;
}

void MqttClient::setClientId(const char* client_id)  {
    checkInstance();
    if(impl != nullptr) {
        impl->setClientId(client_id);
    }
}

void MqttClient::setAuth(const char* username, const char* password) {
    checkInstance();
    if(impl != nullptr) {
        impl->setAuth(username, password);
    }
}

void MqttClient::setWill(Topic willTopic, const uint8_t* will_message, size_t will_size) {
    checkInstance();
    if(impl != nullptr) {
        impl->setWill(willTopic, will_message, will_size);
    }
}

void MqttClient::setReceiveCallback(MqttReceiveCallback cbk) {
    checkInstance();
    if(impl != nullptr) {
        impl->setReceiveCallback(cbk);
    }
}

void MqttClient::checkInstance() {
    if(impl == nullptr && _factory != nullptr) {
        // impl = _factory();
        impl = _factory->operator()();
    }
}

void MqttClient::setFactory(std::function<std::unique_ptr<MqttClientInterface>()> factory) {
    // FIXME find a better way to solve constructor call order
    static std::function<std::unique_ptr<MqttClientInterface>()> f = factory;
    _factory = &f;
    // _factory = factory;
}
