//
// Created by maria on 04.09.2023.
//

#include <iostream>
#include "Networking/server/tcpServer.h"

namespace Multiplayer {
    using boost::asio::ip::tcp;
    tcpServer::tcpServer(IPv ipv, int port) : _ipVersion(ipv), _port(port),
        _acceptor(_ioContext, tcp::endpoint(_ipVersion == IPv::V4 ? tcp::v4() : tcp::v6(), _port))
        // acceptor has no default constructor
    {


    }

    int tcpServer::Run() {
        try {
            startAccept();
            _ioContext.run();
        } catch (std::exception& e){
            std::cerr << e.what() << std::endl;
            return -1;
        }
        return 0;
    }

    void tcpServer::broadcast(const std::string &message) {
        for (auto& connection : _connections) {
            connection->Post(message);
        }
    }

    void tcpServer::startAccept() {
        // emplace creates an object
        _socket.emplace(_ioContext);

        // asynchronously accept the connection
        _acceptor.async_accept(*_socket, [this](const boost::system::error_code& error)
        {
            // Create a connection (move creates the &&-type, which is also called right-hand value)
            auto connection = tcpConnection::Create(std::move(*_socket));

            if (onJoin) {
                onJoin(connection);
            }

            // connections array (unordered_set)
            _connections.insert(connection);

            if (!error) {
                connection->Start(
                        // definition message handler
                        [this](const std::string& message) { if (onClientMessage) onClientMessage(message); },
                        // definition error handler
                        [&, weak= std::weak_ptr(connection)] {
                            // if the connection is still valid
                            if (auto shared = weak.lock();
                            // if we found the same connection in connections-set
                                shared && _connections.erase(shared)) {
                                if (onLeave) onLeave(shared);
                            }
                        }
                    );
            }

            startAccept();
        });
    }
}
