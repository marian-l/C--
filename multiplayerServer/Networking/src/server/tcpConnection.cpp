//
// Created by maria on 04.09.2023.
//

#include <iostream>
#include <boost/asio/write.hpp>
#include <boost/asio/streambuf.hpp>
#include "Networking/server/tcpConnection.h"

namespace Multiplayer {
    tcpConnection::tcpConnection(boost::asio::io_context &ioContext) : _socket(ioContext){

}

void tcpConnection::Start() {
        // "you can create a shared pointer within yourself pointing to yourself
        auto strongThis = shared_from_this();

        boost::asio::async_write(_socket, boost::asio::buffer(_message),
        [strongThis](const boost::system::error_code& error, size_t bytesTransferred) {
            if (error) {
                std::cout << "Failed to send message!\n" << std::endl;
            } else {
                std::cout << "Sent " << bytesTransferred << " bytes of data \n";
            }
    });

    boost::asio::streambuf buffer;

    _socket.async_receive(buffer.prepare(512), [this](const boost::system::error_code& error, size_t bytesTransferred) {
        if (error == boost::asio::error::eof) {
            // clean connection cutoff
            std::cout << "Client disconnected properly!" << std::endl;
        } else if (error){
            // throw boost::system::system_error(error);
            std::cout << "Client disconnected improperly!" << std::endl;
        }
    });
}
}
