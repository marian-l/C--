#include "ListeningSocketObject.hpp"

hde::ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, u_long address)
    : SimpleSocketObject(domain, service, protocol, port, address) 
    {
        establish_net_conn
    }