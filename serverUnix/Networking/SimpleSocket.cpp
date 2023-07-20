// g++ SimpleSocket.cpp -o SimpleSocket.exe -lws2_32

#define SimpleSocket_hpp

#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "wsock32.lib")

#define PORT 8088

// sockaddress is a structure for the initial information which is transmitted with each connection. It is consumed by the OS.
// struct sockaddr_in             //          WINDOWS         struct sockaddr_in       UNIX
// {                              // {
//     ADDRESS_FAMILY sin_family; //    __uint8_t          sin_len;
//     unsigned short sin_port;   //    sa_family_t        sin_family;
//     in_addr sin_addr;          //    in_port_t          sin_port;
//     char sin_zero[8];          //    struct in_addr     sin_addr;
// };                             //    char               sin_zero[8];  };

int main(int argc, char const *argv[])
{
    // Winsocket DLL initialization
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        perror("Failed to initialize Winsock");
        exit(EXIT_FAILURE);
    }

    // Domain/Adress-Family: AF_INET (IPv4) or AF_INET6 (IPv6). AF_UNIX, AF_ISO etc.
    // Type of Service: Selected by needs of Application. Stream, Datagram, RAW.
    // Protocol is Transport Protocol
    int new_socket, server_fd; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    const char *hello = "Hello from server";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    // htons and htonl convert short and long ints, into ports and Addresses
    address.sin_family = AF_INET;
    
    // the s_addr is a member of the in addr structure which is a unsigned long.
    // htonl is a function which converts the it from host byte order to network byte order. 
    // the socket is bound to all available network interfaces
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons( PORT );

    // memset((char *)&address, 0, sizeof(address));
    // memset(address.sin_zero, '\0', sizeof address.sin_zero);

    // casting sockaddr_in address to sockaddress, with the fields going together
    if (bind(server_fd, (struct sockaddr *)&address, addrlen) < 0)
    {
        perror("bind failed");
        printf("Error code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // backlog is the maximum number of pending connections before refusal
    if (listen(server_fd, 3) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        printf("Waiting for new Connection...");
        // this is system call for the first connection request in the queue
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (int *)&addrlen)) < 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[1024] = {0};
        int valread = recv(new_socket, buffer, sizeof(buffer) - 1, 0);
        printf("%s\n", buffer);
        if (valread < 0)
        {
            printf("No bytes are there to read");
        }
        const char *hello = "Hello from the server";
        send(new_socket, hello, strlen(hello), 0);

        closesocket(new_socket);
    }
    return 0;
}