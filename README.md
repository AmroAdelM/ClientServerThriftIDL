# Hello World Thrift Application

This is a simple "Hello World" application using Apache Thrift with C++. It demonstrates how to define a Thrift service, generate C++ code from the Thrift IDL, and implement both the server and client.

## Prerequisites

- Apache Thrift
- C++ compiler (g++)
- Linux environment (e.g., Ubuntu)

## Installation

### Step 1: Install Apache Thrift

```sh
sudo apt-get update
sudo apt-get install thrift-compiler libthrift-dev
```
### Step 2: Generate thrift c++ code
```sh
thrift --gen cpp hello.thrift
```
where gen-cpp is the output folder and hello.thrift is our input thrift file

### Step 3: Compile the server and run it
```sh
g++ -std=c++11 -pthread -o HelloServer HelloServer.cpp gen-cpp/HelloService.cpp -lthrift
./HelloServer
```
### Step 4: Compile the client and run it
```sh
g++ -std=c++11 -o HelloClient HelloClient.cpp gen-cpp/HelloService.cpp -lthrift
./HelloClient
```