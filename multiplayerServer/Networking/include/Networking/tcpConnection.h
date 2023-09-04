//
// Created by maria on 04.09.2023.
//

#ifndef CLIENT_TCPCONNECTION_H
#define CLIENT_TCPCONNECTION_H

#include <boost/asio/ip/tcp.hpp>
#include <memory>

namespace Multiplayer {

using boost::asio::ip::tcp;
class tcpConnection : public std::enable_shared_from_this<tcpConnection> {
public:
    using pointer = std::shared_ptr<tcpConnection>; //
    // if the Connection goes out of scope, its only destroyed if there is no more reference to it

    static pointer Create(boost::asio::io_context& ioContext) {
        return pointer(new tcpConnection(ioContext));
    }

    tcp::socket& socket() {
        return _socket;
    }

    void Start();
private:
    explicit tcpConnection(boost::asio::io_context& ioContext);

private:
    tcp::socket _socket;
    std::string _message {"Client connected! Sending messages! \n"};
};
}
#endif //CLIENT_TCPCONNECTION_H
