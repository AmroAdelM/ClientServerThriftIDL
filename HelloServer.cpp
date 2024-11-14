#include <iostream>
#include "gen-cpp/HelloService.h"
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace hello;
class HelloServiceHandler : public HelloServiceIf {
public:
    HelloServiceHandler() = default;

    void sayHello(std::string& _return, const std::string& name) override {
        _return = "Recieved: " + name + "!";
    }
};

int main() {
    int port = 9090;
    auto handler = std::make_shared<HelloServiceHandler>();
    auto processor = std::make_shared<HelloServiceProcessor>(handler);
    auto serverTransport = std::make_shared<TServerSocket>(port);
    auto transportFactory = std::make_shared<TBufferedTransportFactory>();
    auto protocolFactory = std::make_shared<TBinaryProtocolFactory>();

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    std::cout << "Starting the server..." << std::endl;
    server.serve();
    std::cout << "Server stopped." << std::endl;
    return 0;
}

