//
// Created by casta on 07/04/2023.
//

#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include <boost/asio.hpp>

namespace worker_space {
    std::string getTimestamp();
    class workerThread {
    public:
        static void run(std::shared_ptr<boost::asio::io_service> io_service);

    private:
        static std::mutex m;
    };
}
#endif //WORKER_THREAD_H
