//
// Created by casta on 06/04/2023.
//

#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

#include "settings.hpp"

#include <array>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <boost/asio.hpp>

class participant {
public:
    virtual ~participant() {}

    virtual void onMessage(std::array<char, MAX_IP_PACK_SIZE> &msg) = 0;
};

class chatRoom {
public:
    void enter(std::shared_ptr<participant> participant, const std::string & nickname);
    void leave(std::shared_ptr<participant> participant);
    void broadcast(std::array<char, MAX_IP_PACK_SIZE>& msg, std::shared_ptr<participant> participant);
    std::string getNickname(std::shared_ptr<participant> participant);

private:
    enum { max_recent_msgs = 100 };
    std::unordered_set<std::shared_ptr<participant>> participants_;
    std::unordered_map<std::shared_ptr<participant>, std::string> name_table_;
    std::deque<std::array<char, MAX_IP_PACK_SIZE>> recent_msgs_;
};

namespace worker_space {
    std::string getTimestamp();
    class workerThread {
    public:
        static void run(std::shared_ptr<boost::asio::io_service> io_service);

    private:
        static std::mutex m;
    };
}


#endif //CHAT_ROOM_H
