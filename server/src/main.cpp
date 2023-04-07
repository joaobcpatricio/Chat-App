//
// Created by casta on 06/04/2023.
//

#include "server_app.hpp"
#include "worker_thread.h"

#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/ip/host_name.hpp>
#include <boost/asio/ip/address.hpp>

void runServer(int &argc, char *argv[]) {
    std::shared_ptr<boost::asio::io_service> io_service(
            new boost::asio::io_service);   //smart pointer for the io_service instance
    boost::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(
            *io_service));  //this will prevent io_service from stopping due to out of work
    boost::shared_ptr<boost::asio::io_service::strand> strand(
            new boost::asio::io_service::strand(*io_service));    //provides thread safety for the io_service
    std::cout << "[" << std::this_thread::get_id() << "]" << " Server started" << std::endl;

    try {
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::resolver resolver(io_context);
        boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), "localhost", "");
        boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(
                query);  //resolve the hostname of localhost to the IPv4 endpoint
        //Get local IP to print as an extra info
        boost::asio::ip::tcp::endpoint endpoint = *it;
        boost::asio::ip::address address = endpoint.address();
        std::cout << "My local IP address is: " << address.to_string() << std::endl;
    }
    catch (std::exception &e) {
        std::cerr << "ERR: Exception: " << e.what() << std::endl;
    }

    std::list<std::shared_ptr<server >> servers;
    for (int i = 1; i < argc; ++i) {    //Create server object per port(room)
        std::cout << "Opening room at port " << argv[i] << std::endl;
        tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
        std::shared_ptr<server> a_server(new server(*io_service, *strand, endpoint));
        servers.push_back(a_server);
    }

    boost::thread_group workers;
    for (int i = 0; i < 1; ++i) {   //Change here to increase no. of threads
        boost::thread *t = new boost::thread{boost::bind(&worker_space::workerThread::run, io_service)};

#ifdef __linux__
        //To avoid cache misses and reduce the overhead caused by thread migration between CPU cores
        // bind cpu affinity for worker thread in linux
            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            CPU_SET(i, &cpuset);
            pthread_setaffinity_np(t->native_handle(), sizeof(cpu_set_t), &cpuset);
#endif
        workers.add_thread(t);
    }

    workers.join_all(); //Blocks current thread until all workers`s threads have finished (aka servers execution are stopped)
}

int main(int argc, char *argv[]) {
    try {
        if (argc < 2) {
            std::string default_port = DEFAULT_PORT;
            char *default_args[] = {argv[0], const_cast<char *>(default_port.c_str())};
            argc = sizeof(default_args) / sizeof(default_args[0]);
            argv = default_args;
            std::cout
                    << "WARN: Wrong expected inputs, rooms might not be initialized properly. Using default values.\nProper usage: chat_server <port> [<port> ...]\n";
            runServer(argc, argv);
        } else {
            runServer(argc, argv);
        }

    } catch (std::exception &e) {
        std::cerr << "ERR:  Exception: " << e.what() << "\n";
    }
    std::cout << "Press any key to exit...";
    system("pause");
    return 0;
}
