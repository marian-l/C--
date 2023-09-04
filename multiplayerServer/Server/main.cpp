#include <iostream>
#include <Networking/tcpServer.h>

int main(int argc, char* argv[]) {
    Multiplayer::tcpServer server{Multiplayer::IPv::V4, 1337};

    server.Run();

    return 0;
}