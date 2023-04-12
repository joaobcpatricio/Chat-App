//
// Created by casta on 06/04/2023.
//


#include "server_app.hpp"

#include <iostream>
#include <boost/bind.hpp>

server::server(boost::asio::io_context &io_context,
               boost::asio::io_context::strand &strand,
               const tcp::endpoint &endpoint)
        : io_context_(io_context), strand_(strand), acceptor_(io_context, endpoint) {
    run();
}

void server::run() {
    std::shared_ptr<userInRoom> new_user(new userInRoom(io_context_, strand_, room_));
    acceptor_.async_accept(new_user->socket(),
                           strand_.wrap(boost::bind(&server::onAccept, this, new_user, _1)));
}

void server::onAccept(std::shared_ptr<userInRoom> new_user, const boost::system::error_code &error) {
    if (!error) {
        new_user->start();
    }
    run();
}



