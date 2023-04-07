//
// Created by casta on 06/04/2023.
//

#include "server_user.hpp"

#include <memory>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

userInRoom::userInRoom(boost::asio::io_service& io_service,
                       boost::asio::io_service::strand& strand, chatRoom& room)
        : socket_(io_service), strand_(strand), room_(room)
{
}

tcp::socket& userInRoom::socket() { return socket_; }

void userInRoom::start()
{
    boost::asio::async_read(socket_,
                            boost::asio::buffer(username_, username_.size()),
                            strand_.wrap(boost::bind(&userInRoom::usernameHandler, shared_from_this(), _1)));
}

void userInRoom::onMessage(std::array<char, MAX_IP_PKT_SIZE>& msg)
{
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
    {
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front(), write_msgs_.front().size()),
                                 strand_.wrap(boost::bind(&userInRoom::writeHandler, shared_from_this(), _1)));
    }
}

//Private

void userInRoom::usernameHandler(const boost::system::error_code& error)
{
    if (strlen(username_.data()) <= MAX_USERNAME - 2)
    {
        strcat(username_.data(), ": ");
    }
    else
    {
        //cut off username if too long
        username_[MAX_USERNAME - 2] = ':';
        username_[MAX_USERNAME - 1] = ' ';
    }

    room_.enter(shared_from_this(), std::string(username_.data()));

    boost::asio::async_read(socket_,
                            boost::asio::buffer(read_msg_, read_msg_.size()),
                            strand_.wrap(boost::bind(&userInRoom::readHandler, shared_from_this(), _1)));
}

void userInRoom::readHandler(const boost::system::error_code& error)
{
    if (!error)
    {
        room_.broadcast(read_msg_, shared_from_this());

        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_, read_msg_.size()),
                                strand_.wrap(boost::bind(&userInRoom::readHandler, shared_from_this(), _1)));
    }
    else
    {
        room_.leave(shared_from_this());
    }
}

void userInRoom::writeHandler(const boost::system::error_code& error)
{
    if (!error)
    {
        write_msgs_.pop_front();

        if (!write_msgs_.empty())
        {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(write_msgs_.front(), write_msgs_.front().size()),
                                     strand_.wrap(boost::bind(&userInRoom::writeHandler, shared_from_this(), _1)));
        }
    }
    else
    {
        room_.leave(shared_from_this());
    }
}