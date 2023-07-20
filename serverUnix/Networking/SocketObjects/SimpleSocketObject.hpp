#ifndef SimpleSocketObject_hpp
#define SimpleSocketObject_hpp

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

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