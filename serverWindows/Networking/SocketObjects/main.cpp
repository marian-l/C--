#include <iostream>
#include "hdelibc-sockets.hpp"

int main()
{
    std::cout << "Starting..." << std::endl;
    std::cout << "Simple..." << std::endl;
    hde::SimpleSocketObject ss = hde::SimpleSocketObject(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY);
    std::cout << "Binding..." << std::endl;
    hde::BindingSocket bs = hde::BindingSocket(AF_INET, SOCK_STREAM, 0, 88, INADDR_ANY);
    std::cout << "Listening..." << std::endl;
    hde::ListeningSocket ls = hde::ListeningSocket(AF_INET, SOCK_STREAM, 0, 89, INADDR_ANY, 6);
    
}