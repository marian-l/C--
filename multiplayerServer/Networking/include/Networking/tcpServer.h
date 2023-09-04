//
// Created by maria on 04.09.2023.
//

#ifndef CLIENT_SERVER_H
#define CLIENT_SERVER_H

#include <boost/asio.hpp>
#include "Networking/tcpConnection.h"
#include <functional>

namespace Multiplayer {
    enum class IPv {
        V4,
        V6
    };

    class tcpServer {

    public:
        tcpServer(IPv ipv, int port);

        int Run();

        template <typename T>
        void writeToConnection(int connectionIndex, const T& message);

        template <typename T>
        using ListenCallback = std::function<void(int, const T&)>;

        template <typename T>
        void RegisterListenCallback(ListenCallback<T> callback);

    private:
        void startAccept();
    private:
        IPv _ipVersion;
        int _port;

        boost::asio::io_context _ioContext;
        boost::asio::ip::tcp::acceptor _acceptor;

        std::vector<tcpConnection::pointer> _connections {};
    };
}


#endif //CLIENT_SERVER_H
