//
// Created by maria on 04.09.2023.
//

#include <iostream>
#include <boost/asio/write.hpp>
#include "Networking/tcpConnection.h"
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
}
}
