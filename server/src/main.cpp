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
    std::shared_ptr<boost::asio::io_context> io_context(
            new boost::asio::io_context);
    boost::shared_ptr<boost::asio::io_context::work> work(new boost::asio::io_context::work(
            *io_context));  //this will prevent io_context from stopping due to out of work
    boost::shared_ptr<boost::asio::io_context::strand> strand(
            new boost::asio::io_context::strand(*io_context));    //provides thread safety for the io_context
    std::cout << "[" << std::this_thread::get_id() << "]" << " Server started" << std::endl;

    try {
        boost::asio::ip::tcp::endpoint endpoint = boost::asio::ip::tcp::endpoint(
                boost::asio::ip::make_address_v4("127.0.0.1"), 0);
        boost::asio::ip::address address = endpoint.address();
        std::cout << "My IP address is: " << address.to_string() << std::endl;
    } catch (std::exception& e) {
        std::cerr << "ERR: Exception: " << e.what() << std::endl;
        return;
    }

    const int kNumArgs = argc;
    std::list<std::shared_ptr<server>> servers;
    for (int i = 1; i < kNumArgs; ++i) {
        char* end;
        long port = std::strtol(argv[i], &end, 10);
        if (*end != '\0' || port <= 0 || port > 65535) {
            std::cerr << "ERR: Invalid port number: " << argv[i] << std::endl;
            return;
        }
        std::cout << "Opening room at port " << port << std::endl;
        tcp::endpoint endpoint(tcp::v4(), port);
        std::shared_ptr<server> a_server(new server(*io_context, *strand, endpoint));
        servers.push_back(a_server);
    }

    boost::thread_group workers;
    for (int i = 0; i < 1; ++i) {   //Change here to increase no. of threads
        boost::thread *t = new boost::thread{boost::bind(&worker_space::workerThread::run, io_context)};
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
                    << "WARN: Wrong expected inputs, rooms might not be initialized properly. Using default values.\nProper usage: ServerApp.exe <port> [<port> ...]\n";
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
