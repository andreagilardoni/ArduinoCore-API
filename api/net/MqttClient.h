#pragma once
#include <functional>
#include <memory>

#include <api/Client.h>

// https://github.com/arduino-libraries/ArduinoMqttClient/

// The Idea for this section of the library is to allow the usage of different implementation for Mqtt Clients
// while preserving the possibility of having an Arduino standardized interface for Mqtt protocol
// One should implement MqttClientInterface and provide a way to instantiate the implementation

// namespace arduino { // namespace net { namespace mqtt {

using Topic = const char* const;

// for incoming published messages
// TODO double check with typename
using MqttReceiveCallback = std::function<void(Topic, const uint8_t[], size_t)>;

// TODO define callback for mqtt events. one should be the default, but the user can always change it

typedef int error_t; // TODO move this to be generally available

enum MqttQos: uint8_t {
    MqttQos0 = 0, // At Most once
    MqttQos1 = 1, // At least once
    MqttQos2 = 2, // Exactly once
};

// TODO define mqtt version

constexpr MqttQos QosDefault = MqttQos0;
constexpr size_t MqttClientIdMaxLength = 256;

class MqttClientInterface: public arduino::ClientConnect{
public:
    // virtual ~MqttClientInterface() = default; // not needed if deriving from ClientConnect

    virtual error_t subscribe(Topic t, MqttQos qos = QosDefault) = 0;
    virtual error_t publish(Topic t,  uint8_t payload[], size_t size, MqttQos qos = QosDefault) = 0;
    // TODO define a publish that can stream a message

    virtual error_t unsubscribe(Topic t) = 0;
    virtual void poll() = 0;
    virtual error_t ping() = 0;

    virtual void setReceiveCallback(MqttReceiveCallback cbk) { _cbk = cbk; }

    // nullptr means generate it randomly
    // TODO should this be pure virtual?
    virtual void setClientId(const char* const clientid = nullptr) { // TODO put this in .cpp file
        if(clientid == nullptr) {
            // TODO generate it randomly
        } else {
            strncpy(_clientid, clientid, MqttClientIdMaxLength);
        }
    }

    // TODO Will stuff
    // TODO auth stuff, also related to MQTT 5.0
protected:
    char _clientid[MqttClientIdMaxLength+1];

    // TODO single callback for every incoming message or a callback for everything?
    MqttReceiveCallback _cbk;
};


// this could be generally available, outside of namespaces
class MqttClient: public MqttClientInterface {
public:

    int connect(IPAddress ip, uint16_t port) override;
    int connect(const char *host, uint16_t port) override;
    void disconnect() override;

    uint8_t connected() override;
    operator bool() override;

    //TODO wrap method
    error_t subscribe(Topic t, MqttQos qos = QosDefault) override;
    error_t publish(Topic t, uint8_t[], size_t, MqttQos qos = QosDefault) override;
    error_t unsubscribe(Topic t) override;
    void poll() override;
    error_t ping() override;

    static void setFactory(std::function<std::unique_ptr<MqttClientInterface>()> factory) {
        _factory = factory;
    }
protected:
    static std::function<std::unique_ptr<MqttClientInterface>()> _factory;

    std::unique_ptr<MqttClientInterface> impl;
};

// } // }}
