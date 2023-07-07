#include "BindingSocketObject.hpp"

hde::BindingSocket::BindingSocket(int domain, int service, int protocol, int port, u_long address) 
    : SimpleSocketObject(domain, service, protocol, port, address)
    {
        set_connection(establish_net_conn(get_sock(), get_address()));
        test_connection(get_connection());
    }

int hde::BindingSocket::establish_net_conn(int sock, struct sockaddr_in address)
{
    return bind(get_sock(), (struct sockaddr *)& address, sizeof(address));
}