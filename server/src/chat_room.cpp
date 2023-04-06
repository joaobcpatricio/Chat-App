//
// Created by casta on 06/04/2023.
//

#include "chat_room.hpp"

#include <algorithm>
#include <boost/bind.hpp>
#include <iostream>

void chatRoom::enter(std::shared_ptr <participant> participant, const std::string &nickname) {
    participants_.insert(participant);
    name_table_[participant] = nickname;
    std::for_each(recent_msgs_.begin(), recent_msgs_.end(),
                  boost::bind(&participant::onMessage, participant, _1));
}

void chatRoom::leave(std::shared_ptr <participant> participant) {
    participants_.erase(participant);
    name_table_.erase(participant);
}

void chatRoom::broadcast(std::array<char, MAX_IP_PACK_SIZE> &msg, std::shared_ptr <participant> participant) {
    std::string timestamp = worker_space::getTimestamp();
    std::string nickname = getNickname(participant);
    std::array<char, MAX_IP_PACK_SIZE> formatted_msg;

    // boundary correctness is guarded by protocol.hpp
    strcpy(formatted_msg.data(), timestamp.c_str());
    strcat(formatted_msg.data(), nickname.c_str());
    strcat(formatted_msg.data(), msg.data());

    recent_msgs_.push_back(formatted_msg);
    while (recent_msgs_.size() > max_recent_msgs) {
        recent_msgs_.pop_front();
    }

    std::for_each(participants_.begin(), participants_.end(),
                  boost::bind(&participant::onMessage, _1, std::ref(formatted_msg)));
}

std::string chatRoom::getNickname(std::shared_ptr <participant> participant) {
    return name_table_[participant];
}


namespace worker_space {
    std::string getTimestamp() {
        time_t t = time(0);   // get time now
        struct tm *now = localtime(&t);
        std::stringstream ss;
        ss << '[' << (now->tm_year + 1900) << '-' << std::setfill('0')
           << std::setw(2) << (now->tm_mon + 1) << '-' << std::setfill('0')
           << std::setw(2) << now->tm_mday << ' ' << std::setfill('0')
           << std::setw(2) << now->tm_hour << ":" << std::setfill('0')
           << std::setw(2) << now->tm_min << ":" << std::setfill('0')
           << std::setw(2) << now->tm_sec << "] ";

        return ss.str();
    }


    void workerThread::run(std::shared_ptr<boost::asio::io_service> io_service) {
        {
            std::lock_guard<std::mutex> lock(m);
            std::cout << "[" << std::this_thread::get_id() << "] Thread starts" << std::endl;
        }

        io_service->run();

        {
            std::lock_guard<std::mutex> lock(m);
            std::cout << "[" << std::this_thread::get_id() << "] Thread ends" << std::endl;
        }
    }
    std::mutex workerThread::m;
}

