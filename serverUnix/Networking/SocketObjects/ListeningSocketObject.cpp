#include "ListeningSocketObject.hpp"

hde::ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, u_long address, int backlog)
    : BindingSocket(domain, service, protocol, port, address) 
    {
        listening = listen(get_sock(), backlog);
        test_connection(listening);
    };