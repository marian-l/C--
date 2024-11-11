//
// Created by maria on 14.09.2023.
//

#include "Networking/client/tcp_client.h"

namespace Multiplayer {

    Multiplayer::TCPClient::TCPClient(const std::string &address, int port) : _socket(_ioContext) { // no default constructor for socket
        // populate endpoints by creating resolver
        io::ip::tcp::resolver resolver {_ioContext};
        _endpoints = resolver.resolve(address, std::to_string(port));
    }

    void Multiplayer::TCPClient::Run() {

        io::async_connect(_socket, _endpoints, [this](boost::system::error_code ec, io::ip::tcp::endpoint ep) { // [this] is a lambda function
            if(!ec)
                asyncRead();
        });

        // runs pretty much forever
        _ioContext.run();
    }

    // close the socket
    void Multiplayer::TCPClient::Stop() {
        boost::system::error_code ec;
        _socket.close(ec);

        if (ec) {
            // process error
        }
    }

    void Multiplayer::TCPClient::Post(const std::string &message) {
        bool queueIdle = _outgoingMessage.empty();
        _outgoingMessage.push(message);

        if (queueIdle) {
            asyncWrite();
        }
    }

    void Multiplayer::TCPClient::asyncRead() {
        io::async_read_until(_socket, _streamBuffer, "\n", [this](boost::system::error_code ec, size_t bytesTransferred) {
            onRead(ec, bytesTransferred);
        });
    }

    void Multiplayer::TCPClient::onRead(boost::system::error_code ec, size_t bytesTransferred) {
        if (ec) {
            Stop();
            return;
        }

        std::stringstream message;
        message << std::istream{&_streamBuffer}.rdbuf();
        onMessage(message.str());
        asyncRead();
    }

    void Multiplayer::TCPClient::asyncWrite() {
        io::async_write(_socket, io::buffer(_outgoingMessage.front()), [this](boost::system::error_code ec, size_t bytesTransferred) {
            onWrite(ec, bytesTransferred);
        });
    }

    void Multiplayer::TCPClient::onWrite(boost::system::error_code ec, size_t bytesTransferred) {
        if(ec) {
            Stop();
            return;
        }

        _outgoingMessage.pop();

        if (!_outgoingMessage.empty()) {
            asyncWrite();
        }
    }
}