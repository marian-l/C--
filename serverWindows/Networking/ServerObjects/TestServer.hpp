#ifndef TestServer_hpp
#define TestServer_hpp

#include <stdio.h>
#include "SimpleServer.hpp"
#include <string.h>

namespace hde
{
    class TestServer: public SimpleServer
    {
        private:
            int address;
            int new_socket;
            long i;
            char buffer[30000] = {0};

            void accepter();
            void handler();
            void responder();
        public:
            TestServer();

            void launch();
    };
}

#endif