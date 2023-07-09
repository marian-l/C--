#include "TestServer.hpp"
#include "SimpleServer.hpp"
#include <string.h>

hde::TestServer::TestServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
{
    launch();
}

void hde::TestServer::accepter()
{
    // access the member of the socket object
    struct sockaddr_in address = get_socket()->get_address();
    int addrlen = sizeof(address);
    new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (int *) addrlen);

    recv(new_socket, buffer, sizeof(buffer) - 1, 0);
}

void hde::TestServer::handler()
{
    printf(buffer);
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
        accepter();
        responder();
        launch();
    }
}