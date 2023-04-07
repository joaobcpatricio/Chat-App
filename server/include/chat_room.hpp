//
// Created by casta on 06/04/2023.
//

#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

#include "settings.hpp"
#include "worker_thread.h"

#include <array>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <boost/asio.hpp>

class participant {
public:
    virtual ~participant() = default;

    virtual void onMessage(std::array<char, MAX_IP_PKT_SIZE> &msg) = 0;
};

class chatRoom {
public:
    void enter(std::shared_ptr<participant> participant, const std::string & username);
    void leave(std::shared_ptr<participant> participant);
    void broadcast(std::array<char, MAX_IP_PKT_SIZE>& msg, std::shared_ptr<participant> participant);
    std::string getUsername(std::shared_ptr<participant> participant);

private:
    enum { max_recent_msgs = 100 };
    std::unordered_set<std::shared_ptr<participant>> participants_;
    std::unordered_map<std::shared_ptr<participant>, std::string> name_table_;
    std::deque<std::array<char, MAX_IP_PKT_SIZE>> recent_msgs_;
};



#endif //CHAT_ROOM_H
