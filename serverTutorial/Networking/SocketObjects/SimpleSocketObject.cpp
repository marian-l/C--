#include "SimpleSocketObject.hpp"

hde::SimpleSocketObject::SimpleSocketObject(int domain, int service, int protocol, int port, u_long address_space) 
{
    //Winsocket DLL Initialization
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        perror("Failed to initialize Winsock");
        exit(EXIT_FAILURE);
    }

    // Address Structure which is gonna be casted and referenced to
    address.sin_family = domain;
    address.sin_addr.s_addr = htonl(address_space);
    address.sin_port = htons(port);
    addrlen = sizeof(address);

    // Establish connection
    sock = socket(domain, service, protocol); // scope currently just in constructor
    test_connection(sock);
}

hde::SimpleSocketObject::~SimpleSocketObject() {
    // Clean up resources
    closesocket(sock);
    WSACleanup();
}

int hde::SimpleSocketObject::establish_net_conn(int sock, struct sockaddr_in address)
{
    int number = 0;
    return number;
}

void hde::SimpleSocketObject::test_connection(int item_to_test)
{
    if (item_to_test < 0)
    {
        perror("Failed to connect....");
        exit(EXIT_FAILURE);
    }
}

struct sockaddr_in hde::SimpleSocketObject::get_address()
{
    return address;
}

int hde::SimpleSocketObject::get_connection()
{
    return connection;
}

int hde::SimpleSocketObject::get_sock() 
{
    return sock;
}

int hde::SimpleSocketObject::get_addrlen()
{
    return addrlen;
}


void hde::SimpleSocketObject::set_connection(int con)
{
    connection = con;
}