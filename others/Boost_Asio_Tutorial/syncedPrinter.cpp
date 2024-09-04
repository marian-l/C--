//
// Created by maria on 15.11.2023.
//
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>

class syncedPrinter {
public:
    syncedPrinter(boost::asio::io_context& ioContext) :
    strand_(boost::asio::make_strand(ioContext)),
    timer1_(ioContext, boost::asio::chrono::seconds(1)),
    timer2_(ioContext, boost::asio::chrono::seconds(1)),
    count_(0)
    {
        timer1_.async_wait(boost::asio::bind_executor(strand_, boost::bind(&syncedPrinter::print1, this)));
        timer2_.async_wait(boost::asio::bind_executor(strand_, boost::bind(&syncedPrinter::print2, this)));
    }

    void print1()
    {
        if (count_ < 10)
        {
            std::cout << "Timer 1: " << count_ << std::endl;
            ++count_;

            timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));

            timer1_.async_wait(boost::asio::bind_executor(strand_,
                                                          boost::bind(&syncedPrinter::print1, this)));
        }
    }

    void print2()
    {
        if (count_ < 10)
        {
            std::cout << "Timer 2: " << count_ << std::endl;
            ++count_;

            timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(1));

            timer2_.async_wait(boost::asio::bind_executor(strand_,
                                                          boost::bind(&syncedPrinter::print2, this)));
        }
    }

private:
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    boost::asio::steady_timer timer2_;
    boost::asio::steady_timer timer1_;
    int count_;
};