//
// Created by maria on 04.09.2023.
//

#ifndef CLIENT_SERVER_H
#define CLIENT_SERVER_H

#include <boost/asio.hpp>
#include <optional>
#include <functional>
#include <unordered_set>
#include "tcpConnection.h"

namespace Multiplayer {
    namespace io = boost::asio;

    enum class IPv {
        V4,
        V6
    };

    class tcpServer {
        // if someone joins, this is triggered giving it the connection
        using onJoinHandler = std::function<void(tcpConnection::pointer)>;
        using onLeaveHandler = std::function<void(tcpConnection::pointer)>;
        using onClientMessageHandler = std::function<void(std::string)>;

    public:
        tcpServer(IPv ipv, int port);

        int Run();

        void broadcast(const std::string& message);
    private:
        void startAccept();
    public:
        onJoinHandler onJoin;
        onLeaveHandler onLeave;
        onClientMessageHandler onClientMessage;
    private:
        IPv _ipVersion;
        int _port;

        io::io_context _ioContext;
        io::ip::tcp::acceptor _acceptor;

        std::optional<io::ip::tcp::socket> _socket;
        std::unordered_set<tcpConnection::pointer> _connections {};
    };
}

#endif //CLIENT_SERVER_H
