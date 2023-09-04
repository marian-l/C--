//
// Created by maria on 04.09.2023.
//

#ifndef CLIENT_SERVER_H
#define CLIENT_SERVER_H

#include <boost/asio.hpp>

namespace Multiplayer {
    enum class IPv {
        V4,
        V6
    };

    class tcpServer {

    public:
        tcpServer(IPv ipv, int port);

        int Run();
    private:
        void startAccept();
    private:
        IPv _ipVersion;
        int _port;

        boost::asio::io_context _ioContext;
        boost::asio::ip::tcp::acceptor _acceptor;
    };
}


#endif //CLIENT_SERVER_H
