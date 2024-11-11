//
// Created by maria on 04.09.2023.
//

#include <iostream>
#include <boost/asio/write.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/read_until.hpp>
#include "Networking/server/tcpConnection.h"

namespace Multiplayer {
    namespace io = boost::asio;

    tcpConnection::tcpConnection(io::ip::tcp::socket&& socket) : _socket(std::move(socket)) {
        boost::system::error_code ec;

        std::stringstream name;
        name << _socket.remote_endpoint();

        _username = name.str();
    }

    void tcpConnection::Start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler) {
        _messageHandler = std::move(messageHandler);
        _errorHandler = std::move(errorHandler);

        asyncRead();
    }

    // queues messages on request (if the client sends)
    void tcpConnection::Post(const std::string &message) {
        bool queueIdle = _outgoingMessages.empty(); // outgoingMessages is a queue!
        _outgoingMessages.push(message);

        if(queueIdle) {
            asyncWrite();
        }
    }

    void tcpConnection::asyncRead() {
        io::async_read_until(_socket, _streamBuffer, "\n", [self = shared_from_this()]
                (boost::system::error_code ec, size_t bytesTransferred) {
            self->onRead(ec, bytesTransferred);
        });
    }

    void tcpConnection::onRead(boost::system::error_code ec, size_t bytesTransferred) {
        if(ec) {
            _socket.close(/*ec*/); // check for error

            _errorHandler();
            return;
        }

        std::stringstream message;
        message << _username << ": " << std::istream(&_streamBuffer).rdbuf();
        _streamBuffer.consume(bytesTransferred);
        // muss nicht gecallt werden, da es von rdbuf() bereits gecallt wird. entfernt die angegebene Zahl Bytes aus dem Buffer.

        _messageHandler(message.str());
        asyncRead();
    }

    void tcpConnection::asyncWrite() {
        // we create a buffer from front object of queue.
        io::async_write(_socket, io::buffer(_outgoingMessages.front()), [self = shared_from_this()]
                (boost::system::error_code ec, size_t bytesTransferred) {
            self->onWrite(ec, bytesTransferred);
        });
    }

    void tcpConnection::onWrite(boost::system::error_code ec, size_t bytesTransferred) {
        if(ec) {
            _socket.close(/*ec*/); // check for error

            _errorHandler();
            return;
        }

        _outgoingMessages.pop();

        if (!_outgoingMessages.empty()) {
            asyncWrite();
        }
    }
}
