#include <stdio.h>
#include "Server.hpp"
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>

void launch(struct Server *server)
{
    while (true) {

    char buffer[30000];
    int addrlen = sizeof(server->address);

    int new_socket = accept(server->socket, (struct sockaddr *)&server->address, (socklen_t *)&addrlen);
    if (new_socket == -1) {
        perror("Failed to accept client connection");
        exit(1);
    }

    read(new_socket, buffer, 30000);


    printf("%s\n", buffer);
    char *hello = "HTTP/1.1 200 OK\r\n"
              "Date: Mon, 24 Jul 2023 12:28:53 GMT\r\n"
              "Server: Apache/2.2.14 (win32)\r\n"
              "Last-Modified: Wed, 19 Jul 2023 19:15:56 GMT\r\n"
              "Content-Length: 106\r\n" // Adjusted the Content-Length to 106
              "Content-Type: text/html\r\n"
              "Connection: Closed\r\n\r\n"
              "<html><head><title>Test Server</title></head><body><h1>Hello there</h1></body></html>";
    write(new_socket, hello, strlen(hello));  
    close(new_socket);

    }
}

int main()
{  
    struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 80, 10, launch);
    std::cout << server.socket << std::endl;
    std::cout << inet_ntoa(server.address.sin_addr) << std::endl;
    std::cout << server.interface << std::endl;
    std::cout << server.port << std::endl;

    server.launch(&server);
}