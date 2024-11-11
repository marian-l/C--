//
// Created by maria on 04.09.2023.
//

#ifndef CLIENT_TCPCONNECTION_H
#define CLIENT_TCPCONNECTION_H

#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <queue>

namespace Multiplayer {

    using boost::asio::ip::tcp;
    namespace io = boost::asio;

    using MessageHandler = std::function<void(std::string)>;
    using ErrorHandler = std::function<void()>;

class tcpConnection : public std::enable_shared_from_this<tcpConnection> {
    public:
        using pointer = std::shared_ptr<tcpConnection>; //
        // if the Connection goes out of scope, its only destroyed if there is no more reference to it

        // right hand value socket object as parameter (&&socket)
        static pointer Create(io::ip::tcp::socket&& socket) {
            return pointer(new tcpConnection(std::move(socket)));
        }

        inline const std::string& GetUsername() const { return _username; }
        tcp::socket& socket() {
            return _socket;
        }

        void Start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler);
        void Post(const std::string& message); // message queue

    private:
        explicit tcpConnection(io::ip::tcp::socket&& socket);

        // wait for a new message from the client
        void asyncRead();
        void onRead(boost::system::error_code, size_t bytesTransferred);

        // write a new message to the queue
        void asyncWrite();
        void onWrite(boost::system::error_code, size_t bytesTransferred);

    private:
        tcp::socket _socket;
        std::string _username;

        std::queue<std::string> _outgoingMessages;
        io::streambuf _streamBuffer {65536};

        MessageHandler _messageHandler;
        ErrorHandler _errorHandler;
    };
}
#endif //CLIENT_TCPCONNECTION_H
