//
// Created by casta on 07/04/2023.
//

#include "worker_thread.h"

#include <iostream>

namespace worker_space {
    std::string getTimestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << '[' << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S") << "] ";
        return ss.str();
    }

    void workerThread::run(std::shared_ptr<boost::asio::io_context> io_context) {
        {   //scope guard the lock
            std::lock_guard<std::mutex> lock(m);
            std::cout << "[" << std::this_thread::get_id() << "] Thread started" << std::endl;
        }

        io_context->run();

        {
            std::lock_guard<std::mutex> lock(m);
            std::cout << "[" << std::this_thread::get_id() << "] Thread ended" << std::endl;
        }
    }

    std::mutex workerThread::m;
}