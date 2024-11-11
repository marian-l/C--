//
// Created by maria on 14.09.2023.
//
#include <queue>
#include <boost/asio.hpp>

namespace Multiplayer {
    namespace io = boost::asio;
    using MessageHandler = std::function<void(std::string)>;

    class TCPClient {
    public:
        TCPClient(const std::string& address, int port);

        void Run();
        void Stop();
        void Post(const std::string& message);

    private:
        void asyncRead();
        void onRead(boost::system::error_code ec, size_t bytesTransferred);
        void asyncWrite();
        void onWrite(boost::system::error_code ec, size_t bytesTransferred);

    public:
        MessageHandler onMessage;
    private:
        io::io_context _ioContext {};
        io::ip::tcp::socket _socket;

        io::ip::tcp::resolver::results_type _endpoints;

        io::streambuf _streamBuffer{65536};
        std::queue<std::string> _outgoingMessage {};
    };
}

#ifndef CLIENT_TCP_CLIENT_H
#define CLIENT_TCP_CLIENT_H

#endif //CLIENT_TCP_CLIENT_H
