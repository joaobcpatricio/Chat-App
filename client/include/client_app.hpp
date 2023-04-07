//
// Created by casta on 06/04/2023.
//

#ifndef CLIENT_APP_HPP
#define CLIENT_APP_HPP

#include "settings.hpp"
#include <array>
#include <boost/asio.hpp>
#include <deque>

using boost::asio::ip::tcp;

class clientApp {
public:
    clientApp(const std::array<char, MAX_USERNAME> &username,
              boost::asio::io_service &io_service,
              tcp::resolver::iterator endpoint_iterator);

    void write(const std::array<char, MAX_IP_PKT_SIZE> &msg);

    void close();

    std::array<char, MAX_USERNAME> getUsername();

private:
    void onConnect(const boost::system::error_code &error);

    void writeToSocketQueue(std::array<char, MAX_IP_PKT_SIZE> msg);

    void writeHandler(const boost::system::error_code &error);

    void closeSocket();

    void readHandler(const boost::system::error_code &error);

    boost::asio::io_service &io_service_;
    tcp::socket socket_;
    std::deque<std::array<char, MAX_IP_PKT_SIZE>> write_msgs_;

    std::array<char, MAX_IP_PKT_SIZE> read_msg_;
    std::array<char, MAX_USERNAME> username_;
};

#endif //CLIENT_APP_HPP
