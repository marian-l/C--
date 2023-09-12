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

    }

    void tcpServer::startAccept() {
        // Create a connection
        auto connection = tcpConnection::Create(_ioContext);

        _connections.push_back(connection);

        // asynchronously accept the connection
        _acceptor.async_accept(connection->socket(), [connection, this](const boost::system::error_code& error) {
            if (!error) {
                connection->Start();
            }

            startAccept();

            });

    }

    // Todo
    template<typename T>
    void tcpServer::writeToConnection(int connectionIndex, const T &message) {

    }

    // Todo
    template<typename T>
    void tcpServer::RegisterListenCallback(tcpServer::ListenCallback<T> callback) {

    }
}
