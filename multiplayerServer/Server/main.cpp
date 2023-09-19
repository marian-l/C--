#include <iostream>
#include "Networking/server/tcpServer.h"
#include "Networking/server/tcpConnection.h"

int main(int argc, char* argv[]) {
    Multiplayer::tcpServer server{Multiplayer::IPv::V4, 1337};

    server.onJoin = [](const Multiplayer::tcpConnection::pointer& server) {
        std::cout << "User has joined the server: " << server->GetUsername() << std::endl;
    };

    server.onLeave = [](const Multiplayer::tcpConnection::pointer& server) {
        std::cout << "User has left the server: " << server->GetUsername() << std::endl;
    };

    server.onClientMessage = [&server](const std::string& message) {
        server.broadcast(message);
    };

    server.Run();

    return 0;
}