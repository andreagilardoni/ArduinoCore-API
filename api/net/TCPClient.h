#pragma once

namespace arduino { namespace net {

// In most arduino platforms Client classes are bound to a Network interface by inheritance
// This because there is no routing mechanism that decouples the client from the lower level.
// For this reason we need to differentiate from one client to another by allocating and
// deallocating the concrete client

// TODO what can we do to handle Zephyr/mbedos clients? those are not statically bound to an interface,
//      wrapping them could mean wasting resources:
//      - The general use TCPclient could be a typedef?

// TLS client integration into the wrapper:
//    - PIMPL, which implementation is instantiated only if a method of TLS client is called
//      - Issue that the Flash is always being consumed since it may be instantiated at runtime (MACRO Exclusion?)
//      - RAM is not being consumed for TLS structures unless specifically required
//    - TLS client is not a tcp client 2 classes definition are required
//      - Flash is being used only if the class is being used
//      - Client embedding may be of use (TCPClient -> TLSClient -> HttpClient)
//        + the last layer may be instantiated with default values, but can be still embedded manually
//    - middleware definition: between TCPclient and the user there are multiple middleware layers
//      that act as layer 5/6 of iso/osi stack, TLS is one of them (are there any others?)
//      - sort of client embedding, which may be more flexible
//        + I always instantiate a TCPClient, but if I need Http, I will add it as middleware to the current instance
//          + If HttpClient has different specifics from a TCPClient they might be hard to expose
//    - It could be nice to have an explicit connectSSL in client which enables SSL

// We can make this Client available to platforms with enough resources.

/* The idea of client factories is that they are going to be defined by the platform, so that depending on the network stack
 * implementation one can make use of a generic TCP client. For instance zephyr or mbed core can apply an implementation
 * that is not dependant on the network interface that is being used. on other cores, on which local routing is not implemented
 * the client is bound to the interface, thus depending on which one is the default interface one has to select the proper class
 *
 * Example zephyr, this can be placed in the library SocketWrapper:
 *
 * void __setClientFactory() __attribute__((constructor)) {
 *   TCPClientConnect::setFactory([]() {
 *       new ZephyrClient()
 *     );
 *   });
 * }
 *
 * Example Mbed os, this can be placed in the library Ethernet:
 *
 * void __setClientFactory() __attribute__((constructor)) {
 *   // check that there is no other factory already defined. First defined First used
 *   if(TCPClientConnect::factory != nullptr) {
 *      TCPClientConnect::setFactory([]() {
 *        return unique_ptr(
 *          new EthernetClient()
 *        );
 *      });
 *   }
 * }
 */

class TCPClientConnect: public ClientConnect {
public:

  // default generic TCPClient associated with default net interface
  TCPClientConnect()
  : _factory(TCPClientConnect::_factory) {
  }

  ~TCPClientConnect();

  int connect(IPAddress ip, uint16_t port) override;
  int connect(const char *host, uint16_t port) override;
  void disconnect() override;

  uint8_t connected() override;
  operator bool() override;

protected: // TODO should this be private?
  // the factory should provide a way to allocate and deallocate the client
  // there should be a default factory that uses to use the default network interface
  static std::function<std::unique_ptr<Client>()> _factory;

  // factories can be changed per instance
  std::function<std::unique_ptr<Client>()> _factory;

  // the client should be instantiated when calling connect, if it fails the Client is deleted
  // otherwise it is called when the stop/end call is performed
  // unique_ptr is being used to propagate the allocation and deallocation methods that a factory may provide
  std::unique_ptr<Client> client;

  // TODO should a client keep the Network interface reference?

  friend TCPClient;
};

class TCPClient: public Client, TCPClientConnect {
public:
  size_t write(uint8_t) override;
  size_t write(const uint8_t *buf, size_t size) override;
  int read(uint8_t *buf, size_t size) override;
  void stop() override;
};

}}
