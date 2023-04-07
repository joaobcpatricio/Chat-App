//
// Created by casta on 06/04/2023.
//

#include "chat_room.hpp"

#include <algorithm>
#include <boost/bind.hpp>
#include <iostream>

void chatRoom::enter(std::shared_ptr <participant> participant, const std::string &username) {
    participants_.insert(participant);
    name_table_[participant] = username;
    std::for_each(recent_msgs_.begin(), recent_msgs_.end(),
                  boost::bind(&participant::onMessage, participant, _1));
}

void chatRoom::leave(std::shared_ptr <participant> participant) {
    participants_.erase(participant);
    name_table_.erase(participant);
}

void chatRoom::broadcast(std::array<char, MAX_IP_PKT_SIZE> &msg, std::shared_ptr <participant> participant) {
    std::string timestamp = worker_space::getTimestamp();
    std::string username = getUsername(participant);
    std::array<char, MAX_IP_PKT_SIZE> formatted_msg;

    // boundary correctness is guarded by protocol.hpp
    strcpy(formatted_msg.data(), timestamp.c_str());
    strcat(formatted_msg.data(), username.c_str());
    strcat(formatted_msg.data(), msg.data());

    recent_msgs_.push_back(formatted_msg);
    while (recent_msgs_.size() > max_recent_msgs) {
        recent_msgs_.pop_front();
    }

    std::for_each(participants_.begin(), participants_.end(),
                  boost::bind(&participant::onMessage, _1, std::ref(formatted_msg)));
}

std::string chatRoom::getUsername(std::shared_ptr <participant> participant) {
    return name_table_[participant];
}



