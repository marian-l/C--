#ifndef SimpleSocketObject_hpp
#define SimpleSocketObject_hpp

#include <string.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "wsock32.lib")

namespace hde
{
    class SimpleSocketObject
    {
        private: 
            int sock; 
            int connection;
            int addrlen;
            struct sockaddr_in address;

        public: 
            // Constructor
            SimpleSocketObject(int domain, int service, int protocol, int port, u_long address_space);
            
            //Destructor
            virtual ~SimpleSocketObject();

            // Methods
            virtual int establish_net_conn(int sock, struct sockaddr_in address);
            void test_connection(int);

            // Getters
            struct sockaddr_in get_address();
            int get_sock();
            int get_connection();
            int get_addrlen();

            // Setters
            void set_connection(int con);
    };
} 

#endif /* SimpleSocket_hpp*/