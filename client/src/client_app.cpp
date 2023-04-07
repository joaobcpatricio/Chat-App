//
// Created by casta on 06/04/2023.
//

#include "client_app.hpp"
#include <boost/bind.hpp>
#include <thread>
#include <iostream>

clientApp::clientApp(const std::array<char, MAX_USERNAME> &username,
                     boost::asio::io_service &io_service,
                     tcp::resolver::iterator endpoint_iterator) :
        io_service_(io_service), socket_(io_service) {

    std::copy(username.begin(), username.end(), username_.begin());
    std::fill(read_msg_.begin(), read_msg_.end(), '\0');
    boost::asio::async_connect(socket_, endpoint_iterator, boost::bind(&clientApp::onConnect, this, _1));
}

void clientApp::onConnect(const boost::system::error_code &error) { //write to server the username, read its response
    if (!error) {
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(username_, username_.size()),
                                 boost::bind(&clientApp::readHandler, this, _1));
    }
}

void clientApp::write(const std::array<char, MAX_IP_PKT_SIZE> &msg) {
    io_service_.post(boost::bind(&clientApp::writeToSocketQueue, this, msg));    //async call for the write
}

void clientApp::writeToSocketQueue(std::array<char, MAX_IP_PKT_SIZE> msg) {
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress) {
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front(), write_msgs_.front().size()),
                                 boost::bind(&clientApp::writeHandler, this, _1));
    }
}

void clientApp::writeHandler(const boost::system::error_code &error) {
    if (!error) {
        write_msgs_.pop_front();
        if (!write_msgs_.empty()) {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(write_msgs_.front(), write_msgs_.front().size()),
                                     boost::bind(&clientApp::writeHandler, this, _1));
        }
    } else {
        closeSocket();
    }
}

void clientApp::close() {
    io_service_.post(boost::bind(&clientApp::closeSocket, this));
}

void clientApp::closeSocket() {
    socket_.close();
}

std::array<char, MAX_USERNAME> clientApp::getUsername(){
    return username_;
}

void clientApp::readHandler(const boost::system::error_code &error) {
    std::cout << read_msg_.data() << std::endl;
    if (!error) {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_, read_msg_.size()),
                                boost::bind(&clientApp::readHandler, this, _1));
    } else {
        closeSocket();
    }
}





