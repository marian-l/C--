#ifndef SimpleServer_hpp
#define SimpleServer_hpp

#include <stdio.h>

#include "../SocketObjects/hdelibc-sockets.hpp"

namespace hde
{
    class SimpleServer
    {
        private:
        // Space for a Socket.
            ListeningSocket * socket;

            // implementation is up to the children
            virtual void accepter() = 0;
            virtual void handler() = 0;
            virtual void responder() = 0;
        public:
            // constructor
            SimpleServer(int domain, int service, int protocol, int port, u_long address_interface, int backlog);
            
            virtual void launch() = 0;

            ListeningSocket * get_socket();            
    };
} // namespace hde


#endif