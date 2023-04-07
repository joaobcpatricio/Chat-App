//
// Created by casta on 06/04/2023.
//

#include "server_app.hpp"
#include "chat_room.hpp"

#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include <boost/asio/ip/host_name.hpp>
#include <boost/asio/ip/address.hpp>

int main(int argc, char* argv[])
{
    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: chat_server <port> [<port> ...]\n";
            return 1;
        }

        std::shared_ptr<boost::asio::io_service> io_service(new boost::asio::io_service);
        boost::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(*io_service));
        boost::shared_ptr<boost::asio::io_service::strand> strand(new boost::asio::io_service::strand(*io_service));

        std::cout << "[" << std::this_thread::get_id() << "]" << " Server started" << std::endl;

        try
        {
            boost::asio::io_context io_context;
            boost::asio::ip::tcp::resolver resolver(io_context);
            boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), "localhost", "");
            boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(query);
            boost::asio::ip::tcp::endpoint endpoint = *it;
            boost::asio::ip::address address = endpoint.address();
            std::cout << "My local IP address is: " << address.to_string() << std::endl;
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
        }


        std::list < std::shared_ptr < server >> servers;
        for (int i = 1; i < argc; ++i)
        {
            tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
            std::shared_ptr<server> a_server(new server(*io_service, *strand, endpoint));
            servers.push_back(a_server);
        }

        boost::thread_group workers;
        for (int i = 0; i < 1; ++i)
        {
            boost::thread * t = new boost::thread{ boost::bind(&worker_space::workerThread::run, io_service) };

#ifdef __linux__
            // bind cpu affinity for worker thread in linux
            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            CPU_SET(i, &cpuset);
            pthread_setaffinity_np(t->native_handle(), sizeof(cpu_set_t), &cpuset);
#endif
            workers.add_thread(t);
        }

        workers.join_all();
    } catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
