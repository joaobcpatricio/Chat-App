//
// Created by casta on 06/04/2023.
//


#include "server_app.hpp"

#include <iostream>
#include <boost/bind.hpp>

server::server(boost::asio::io_service &io_service,
               boost::asio::io_service::strand &strand,
               const tcp::endpoint &endpoint)
        : io_service_(io_service), strand_(strand), acceptor_(io_service, endpoint) {
    run();
}

//-- Private --

void server::run() {
    std::shared_ptr<userInRoom> new_participant(new userInRoom(io_service_, strand_, room_));
    acceptor_.async_accept(new_participant->socket(),
                           strand_.wrap(boost::bind(&server::onAccept, this, new_participant, _1)));
}

void server::onAccept(std::shared_ptr<userInRoom> new_participant, const boost::system::error_code &error) {
    if (!error) {
        new_participant->start();
    }
    run();
}



