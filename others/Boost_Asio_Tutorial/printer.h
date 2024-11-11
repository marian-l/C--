//
// Created by maria on 15.11.2023.
//

#ifndef BOOST_ASIO_TUTORIAL_PRINTER_H
#define BOOST_ASIO_TUTORIAL_PRINTER_H

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

class printer {
public:
    // this constructor initiates timer from ioContext. Why does it not do this inside the constructor though?
    printer(boost::asio::io_context &ioContext) : timer_(ioContext, boost::asio::chrono::seconds(1)), count_(0) {
        timer_.async_wait(boost::bind(&printer::print, this));
    }

    ~printer() {
        std::cout << "Final count is " << count_ << std::endl;
    }

    void print() {
        if(count_ < 10) {
            std::cout << count_ << std::endl;
            ++(count_);
            // move the expiring time a second forward
            timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1));

            // consecutive calls of the asynchronous function. recursive at this point
            timer_.async_wait(boost::bind(&printer::print, this));
        }
    }

    private:
        boost::asio::steady_timer timer_;
        int count_;

};

#endif //BOOST_ASIO_TUTORIAL_PRINTER_H
