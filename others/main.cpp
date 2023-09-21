#include <iostream>
#include <boost/asio.hpp>

int main() {
    std::cout << "Hello, World!" << std::endl;
    namespace io = boost::asio;
    io::io_context ioContext;

    io::steady_timer timer(ioContext, io::chrono::seconds(5));

    std::cout << timer.get_executor() << std::endl;

    timer.wait();

    ioContext.run();

    std::cout << "Hello, Code!" << std::endl;

    return 0;
}
