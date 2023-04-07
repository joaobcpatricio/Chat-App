//
// Created by casta on 06/04/2023.
//

#ifndef SERVER_USER_H
#define SERVER_USER_H


#include "settings.hpp"
#include "chat_room.hpp"
#include <boost/asio.hpp>
#include <array>
#include <deque>

using boost::asio::ip::tcp;


class userInRoom : public participant,
                   public std::enable_shared_from_this<userInRoom> {
public:
    userInRoom(boost::asio::io_service &io_service,
               boost::asio::io_service::strand &strand, chatRoom &room);

    tcp::socket &socket();

    void start();

    void onMessage(std::array<char, MAX_IP_PKT_SIZE> &msg);

private:
    void usernameHandler(const boost::system::error_code &error);

    void readHandler(const boost::system::error_code &error);

    void writeHandler(const boost::system::error_code &error);

    tcp::socket socket_;
    boost::asio::io_service::strand &strand_;
    chatRoom &room_;
    std::array<char, MAX_USERNAME> username_;
    std::array<char, MAX_IP_PKT_SIZE> read_msg_;
    std::deque<std::array<char, MAX_IP_PKT_SIZE> > write_msgs_;
};

#endif //SERVER_USER_H
