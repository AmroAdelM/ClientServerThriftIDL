#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include "gen-cpp/HelloService.h"
#include <iostream>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace hello;

int main() {
    auto socket = std::make_shared<TSocket>("localhost", 9090);
    auto transport = std::make_shared<TBufferedTransport>(socket);
    auto protocol = std::make_shared<TBinaryProtocol>(transport);
    HelloServiceClient client(protocol);

    try {
        transport->open();
        std::string result;
        client.sayHello(result, "Hello World");
        std::cout << "Server response: " << result << std::endl;
        transport->close();
    } catch (TException& tx) {
        std::cerr << "ERROR: " << tx.what() << std::endl;
    }

    return 0;
}

