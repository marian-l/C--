#include <iostream>
#include "Networking/client/tcp_client.h"
#include <thread>

using namespace Multiplayer;

int main(int argc, char* argv[]) {
    TCPClient client {"localhost", 1337};

    // Lambda, parameter message wird ausgegeben.
    client.onMessage = [](const std::string& message) {
        std::cout << message;
    };

    std::thread t { [&client] () { client.Run(); }};

    while (true) {
        std::string message;
        getline(std::cin, message);

        if (message == "\\quit" || message == "\\q") break;
        message =+ "\n"; // because getline does not give a linebreak

        client.Post(message);
    }

    client.Stop(); // causes context to stop
    t.join(); // wait for thread to end before closing application

    return 0;
}