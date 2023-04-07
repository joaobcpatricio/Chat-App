//
// Created by casta on 06/04/2023.
//

#include "server_user.hpp"

#include <memory>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>

userInRoom::userInRoom(boost::asio::io_service &io_service,
                       boost::asio::io_service::strand &strand, chatRoom &room)
        : socket_(io_service), strand_(strand), room_(room) {
}

tcp::socket &userInRoom::socket() { return socket_; }

void userInRoom::start() {
    boost::asio::async_read(socket_,
                            boost::asio::buffer(username_, username_.size()),
                            strand_.wrap(boost::bind(&userInRoom::usernameHandler, shared_from_this(), _1)));
}

void userInRoom::informUserJoined() {
    try {
        std::string str(this->username_.data(), this->username_.size() - 2);
        size_t pos = str.find(':');
        std::string enter_msg = str.substr(0, pos) + " joined room ";
        std::cout << enter_msg << this->socket_.local_endpoint().port() << "." << std::endl;
    } catch (std::exception &e) {
        std::cerr << "ERR:  Failed to inform user that entered." << std::endl;
    }
}

void userInRoom::informUserLeft() {
    try {
        std::string str(this->username_.data(), this->username_.size() - 2);
        size_t pos = str.find(':');
        std::string left_msg = str.substr(0, pos) + " left room ";
        std::cout << left_msg << this->socket_.local_endpoint().port() << "." << std::endl;
    } catch (std::exception &e) {
        std::cerr << "ERR:  Failed to inform user that left." << std::endl;
    }
}

void userInRoom::onMessage(std::array<char, MAX_IP_PKT_SIZE> &msg) {
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress) {
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front(), write_msgs_.front().size()),
                                 strand_.wrap(boost::bind(&userInRoom::writeHandler, shared_from_this(), _1)));
    }
}

void userInRoom::usernameHandler(const boost::system::error_code &error) {
    if (strlen(username_.data()) <= MAX_USERNAME - 2) {
        strcat(username_.data(), ": ");
    } else {
        //cut off username if too long
        username_[MAX_USERNAME - 2] = ':';
        username_[MAX_USERNAME - 1] = ' ';
    }

    room_.enter(shared_from_this(), std::string(username_.data()));

    boost::asio::async_read(socket_,
                            boost::asio::buffer(read_msg_, read_msg_.size()),
                            strand_.wrap(boost::bind(&userInRoom::readHandler, shared_from_this(), _1)));
    informUserJoined();
}

void userInRoom::readHandler(const boost::system::error_code &error) {
    if (!error) {
        room_.broadcast(read_msg_, shared_from_this());

        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_, read_msg_.size()),
                                strand_.wrap(boost::bind(&userInRoom::readHandler, shared_from_this(), _1)));
    } else {
        this->informUserLeft();
        room_.leave(shared_from_this());
    }
}

void userInRoom::writeHandler(const boost::system::error_code &error) {
    if (!error) {
        write_msgs_.pop_front();

        if (!write_msgs_.empty()) { //flush all msgs safely with strand_.wrap
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(write_msgs_.front(), write_msgs_.front().size()),
                                     strand_.wrap(boost::bind(&userInRoom::writeHandler, shared_from_this(), _1)));
        }
    } else {
        room_.leave(shared_from_this());
    }
}