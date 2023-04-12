//
// Created by casta on 06/04/2023.
//

#ifndef SERVER_APP_HPP
#define SERVER_APP_HPP


#include "server_user.hpp"
#include "chat_room.hpp"

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class server {
public:
    server(boost::asio::io_context &io_context,
           boost::asio::io_context::strand &strand,
           const tcp::endpoint &endpoint);

private:
    void run();
    void onAccept(std::shared_ptr<userInRoom> new_user, const boost::system::error_code &error);

    boost::asio::io_context &io_context_;
    boost::asio::io_context::strand &strand_;
    tcp::acceptor acceptor_;
    chatRoom room_;
};
#endif //SERVER_APP_HPP
