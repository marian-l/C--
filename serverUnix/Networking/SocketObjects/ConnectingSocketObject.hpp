#ifndef ConnectingSocketObject_hpp
#define ConnectingSocketObject_hpp

#include <stdio.h>
#include "SimpleSocketObject.hpp"

namespace hde 
    {
        class ConnectingSocket: public SimpleSocketObject
        {
            public: 
                ConnectingSocket(int domain, int service, int protocol, int port, u_long address);
                int establish_net_conn(int sock, struct sockaddr_in address);
        };
    }

#endif