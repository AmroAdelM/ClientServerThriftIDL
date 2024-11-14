#include <iostream>
#include "gen-cpp/HelloService.h"
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <mutex>
#include <thread>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace hello;
class HelloServiceHandler : public HelloServiceIf
{
public:
    HelloServiceHandler() = default;

    void sayHello(std::string &_return, const std::string &name) override
    {
        std::cout << "Recieved: " + name << std::endl;
        _return = "You are Welcome too!";
    }
};

class ConnectionCountingHandler : public TServerEventHandler
{
public:
    ConnectionCountingHandler(int &connectionCount, std::mutex &mtx) : connectionCount_(connectionCount), mtx_(mtx) {}

    void *createContext(std::shared_ptr<TProtocol> input, std::shared_ptr<TProtocol> output) override
    {
        std::lock_guard<std::mutex> lock(mtx_);
        std::cout << "New connection established. Total: " << ++connectionCount_ << std::endl;
        return nullptr;
    }

private:
    int &connectionCount_;
    std::mutex &mtx_;
};

int main()
{
    int port = 9090;
    int maxConnections = 5; // Set the maximum number of connections
    int connectionCount = 0;
    std::mutex mtx;
    auto handler = std::make_shared<HelloServiceHandler>();
    auto processor = std::make_shared<HelloServiceProcessor>(handler);
    auto serverTransport = std::make_shared<TServerSocket>(port);
    auto transportFactory = std::make_shared<TBufferedTransportFactory>();
    auto protocolFactory = std::make_shared<TBinaryProtocolFactory>();

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    auto eventHandler = std::make_shared<ConnectionCountingHandler>(connectionCount, mtx);
    server.setServerEventHandler(eventHandler);

    std::cout << "Starting the server..." << std::endl;

    // Run the server in a separate thread
    std::thread serverThread([&server] {
        server.serve();
    });

    // Monitor the connection count
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (connectionCount >= maxConnections-1)
            {
                std::cout << "Max connections reached. Stopping server." << std::endl;
                server.stop();
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Wait for the server thread to finish
    serverThread.join();

    std::cout << "Server stopped." << std::endl;
    return 0;
}
