#ifndef ListeningSocketObject_hpp
#define ListeningSocketObject_hpp

#include <stdio.h>
#include "SimpleSocketObject.hpp"

namespace hde 
    {
        class ListeningSocket: public SimpleSocketObject
        {
            public: 
                ListeningSocket(int domain, int service, int protocol, int port, u_long address);
                int establish_net_conn(int sock, struct sockaddr_in address);
        };
    }

#endif