#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "printer.h"
#include "syncedPrinter.cpp"

namespace io = boost::asio;

int synchronousTimer(io::io_context& ioContext, io::thread_pool& threadPool) {
    io::steady_timer t(ioContext, io::chrono::seconds(2));
    io::steady_timer t2(threadPool, io::chrono::seconds(5));

    std::cout << "start \n";
    // ioContext
    t.wait();
    // 2 seconds in
    std::cout << "halftime \n";
    // threadPool
    t2.wait();
    // 3 seconds later?
    std::cout << "finish! \n";

    return 0;
}

void print(const boost::system::error_code& errorCode) {
    std::cout << "completion handler";
}

int asynchronousTimer(io::io_context& ioContext, io::thread_pool& threadPool) {
    // timer
    // iocontext
    // handler needs to be defined
    // call async_wait handler
    // call run on context (in different order run would immediately terminate because it had no work)

    io::steady_timer t(ioContext, io::chrono::seconds(2));
    io::steady_timer t2(threadPool, io::chrono::seconds(5));

    t.async_wait(&print);
    t2.async_wait(&print);

    ioContext.run();
    // threadPool has no run() method.
    // threadPool steadyTimer is not operated.
    return 0;
}

void printMod(const boost::system::error_code& errorCode,
              io::steady_timer* t,
              int* count) {

    if(*count < 5) {
        std::cout << *count << std::endl;
        ++(*count);
        // move the expiring time a second forward
        t->expires_at(t->expiry() + io::chrono::seconds(1));

        // consecutive calls of the asynchronous function. recursive at this point
        t->async_wait(boost::bind(printMod,
                              io::placeholders::error(),
                              t,
                              count));
    }
}

int asynchronousTimerMod(io::io_context& ioContext) {
    int count = 0;
    io::steady_timer t(ioContext, io::chrono::seconds(1));

    // the first executed async operation
    // boost bind is required to add multiple arguments to the thread.
    t.async_wait(boost::bind(printMod,
                             io::placeholders::error(),
                             &t,
                             &count));
    ioContext.run();

    std::cout << "Final count is " << count << std::endl;

    return 0;
}

int main() {
    io::io_context ioContext;
    io::thread_pool threadPool;

    // synchronousTimer(ioContext, threadPool);
    // asynchronousTimer(ioContext, threadPool);
    // asynchronousTimerMod(ioContext);

    // with a class and its members.
    // printer p(ioContext);
    // ioContext.run();

    // the previous examples executed on one thread, meaning only one handler could run at a time.
    // thread pool kicks in. now synchronisation of the handlers needs to be addressed as they can run concurrently.
    // importing #include <boost/thread/thread.hpp>
    syncedPrinter sp(ioContext);
    boost::thread t(boost::bind(&boost::asio::io_context::run, &ioContext));
    ioContext.run();
    t.join();

    return 0;
}


