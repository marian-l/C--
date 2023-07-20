#ifndef ListeningSocketObject_hpp
#define ListeningSocketObject_hpp

#include <stdio.h>
#include "BindingSocketObject.hpp"

namespace hde 
    {
        class ListeningSocket: public BindingSocket
        {
            private:
                int new_socket;
                int addrlen;
                int listening;
            public: 
                ListeningSocket(int domain, int service, int protocol, int port, u_long address, int backlog);
        };
    }

#endif