#include "TestServer.hpp"
#include "SimpleServer.hpp"
#include "SimpleServer.cpp"
#include "./../SocketObjects/SimpleSocketObject.hpp"
#include <WinSock2.h>
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
    new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (int *)&addrlen); // int * is a 8Byte Integer, while int is only a 2-4 Byte Integer

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
    char *hello = "Hello from Server";
    send(new_socket, hello, strlen(hello), 0);
    closesocket(new_socket);
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