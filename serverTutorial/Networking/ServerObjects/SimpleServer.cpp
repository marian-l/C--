#include "SimpleServer.hpp"

hde::SimpleServer::SimpleServer(int domain, int service, int protocol, int port, u_long address_interface, int backlog)
{
    // Watch out for memory Leakage. 
    socket = new ListeningSocket(domain, service, protocol, port, address_interface, backlog);

    // First: delete socket | Second: socket = new ListeningSocket(); 

}

hde::ListeningSocket * hde::SimpleServer::get_socket()
{
    return socket;
}