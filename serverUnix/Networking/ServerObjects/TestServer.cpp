#include "TestServer.hpp"
#include "SimpleServer.hpp"
#include "SimpleServer.cpp"
#include "./../SocketObjects/SimpleSocketObject.hpp"
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>

hde::TestServer::TestServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
{
    std::cout << "Started Server" << "\n";
    launch();
}

void hde::TestServer::accepter()
{
    // access the member of the socket object
    struct sockaddr_in address = get_socket()->get_address();
    long long addrlen = sizeof(address);
    new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen); 

    // recv(new_socket, buffer, sizeof(buffer) - 1, 0);
    recv(new_socket, buffer, 30000, 0);
}

void hde::TestServer::handler()
{
    std::cout << buffer << std::endl;
    //printf(buffer);
}

void hde::TestServer::responder()
{
    // char *hello = "Hello from Server";
    char *hello = R"( 
        <!DOCTYPE html> <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Document</title>
        </head>
        <body>
            <h1> Hello from Server, Young Padawan! </h1>
        </body>
        </html> 
        )";
    send(new_socket, hello, strlen(hello), 0);
    close(new_socket);
}

void hde::TestServer::launch()
{
    while (true)
    {
        std::cout << "Debug" << std::endl;
        std::cout << get_socket() << std::endl;
        std::cout << get_socket()->get_sock() << std::endl;
//        std::cout << get_socket()->get_address() << std::endl;
        std::cout << get_socket()->get_addrlen() << std::endl;
        std::cout << get_socket()->get_connection() << std::endl;

        accepter();
        handler();
        responder();
    }
}