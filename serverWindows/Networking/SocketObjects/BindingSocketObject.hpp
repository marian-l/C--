#ifndef BindingSocketObject_hpp
#define BindingSocketObject_hpp

#include <stdio.h>
#include "SimpleSocketObject.hpp"

namespace hde 
    {
        class BindingSocket: public SimpleSocketObject
        {
            public: 
                BindingSocket(int domain, int service, int protocol, int port, u_long address);
                int establish_net_conn(int sock, struct sockaddr_in address);
        };
    }

#endif