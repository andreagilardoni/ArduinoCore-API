#pragma once

#include "../IPAddress.h"

#ifndef MAX_NETIFACES
#define MAX_NETIFACES 4
#endif

class NetworkInterfaceManager {
public:
    static NetworkInterface* getIface(size_t i) { return netiface[i]; }

    NetworkInterfaceManager& getInstance() {
        static NetworkInterfaceManager nim;
        return nim;
    }

private:
    NetworkInterfaceManager(): default_netif(nullptr) {
        memset(&netifaces, 0, sizeof(netifaces));
    }

    ~NetworkInterfaceManager() = default;

    class NetworkInterface;

    // should this be a std::vector? generally only one net interface will be used
    NetworkInterface* netifaces[MAX_NETIFACES];
    NetworkInterface* default_netif;

    void appendIface(NetworkInterface* netif) {
        for (NetworkInterface* ptr = netifaces;
                ptr < netifaces + MAX_NETIFACES;
                ptr++;) {
            if(ptr == nullptr) {
                ptr = netif;

                if(default_netif == nullptr) {
                    default_netif = netif;
                }
            }
        }
    }

    friend NetworkInterface;

public:
    void setDefault(size_t i) {
        if(i < MAX_NETIFACES) {
            default_netif = netifaces[i];
        }
    }

    NetworkInterface* getDefault() {
        return default_netif;
    }
};

// this requires cpp17
// TODO name?
inline NetworkInterfaceManager& NetIfaceManager = NetworkInterfaceManager::getInstance();

// TODO there are different kinds of network interfaces, should we define an enum containing the values from
//      ConnectionHandler?
//
// TODO this is the base level for a Network interface, specific implementations with kind specific
//      methods should also be defined, i.e. Wifi, Wifi sta + Wifi ap, Ethernet, GSM
//
// TODO should we append the network interface only when begin happened with success? how do we know that?
//
// TODO should a network interface also have a ping method?
//
// TODO define explicit poll method
class NetworkInterface {
public:
    NetworkInterface() {
        NetworkInterfaceManager.getInstance().appendIface(this);
    }
    virtual ~NetworkInterface() = default;

    virtual IPAddress localIP() = 0
    virtual IPAddress subnetMask() = 0;
    virtual IPAddress gatewayIP() = 0;
    virtual IPAddress dnsServerIP() = 0;

    // TODO define how a mac address should be represented, 8 bytes/6 bytes
    virtual void setMACAddress(const uint8_t* mac) = 0;
    virtual void getMACAddress(const uint8_t* mac) = 0;

    // A concrete network interface may have different settings that can be plugged in the begin call
    // so there will be overloaded begin calls
    virtual bool begin() = 0;
    virtual bool end() = 0;
protected:
    // NOTE return autopointer with implementation of a client class?
};

