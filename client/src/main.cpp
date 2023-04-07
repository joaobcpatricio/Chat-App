//
// Created by casta on 06/04/2023.
//

#include "client_app.hpp"
#include <boost/bind.hpp>
#include <iostream>
#include <chrono>
#include <string>

void connectToServer(char *argv[]) {
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(argv[2], argv[3]);
    tcp::resolver::iterator iterator = resolver.resolve(query);
    std::array<char, MAX_USERNAME> username;
    strcpy(username.data(), argv[1]);

    clientApp cli(username, io_service,
                  iterator);  // Attempt to connect to the server and create a clientApp instance
    std::cout << "Successfully connected!\nYou can now start chatting."
              << std::endl;  //If not it will throw an exception

    std::thread t(boost::bind(&boost::asio::io_service::run, &
            io_service));

    std::array<char, MAX_IP_PKT_SIZE> msg;

    while (true) {
        memset(msg.data(), '\0', msg.size());
        if (!std::cin.getline(msg.data(), MAX_IP_PKT_SIZE - PADDING - MAX_USERNAME)) {
            std::cin.clear(); //clean up error bit and try to finish reading
        }
        if (std::strcmp(msg.data(), "disconnect") == 0) {
            std::cout << "Disconnecting from the server!" << std::endl;
            std::sprintf(msg.data(), "USER LEFT THE ROOM.");
            cli.write(msg);
            break;
        }
        cli.write(msg);
    }

    cli.close();
    io_service.stop();
    t.join();
}

int main(int argc, char *argv[]) {
    try {

        if (argc < 4) { //Use defaults if missing one of the 3 inputs
            std::string default_username = "Anonymous";
            std::string default_host = "127.0.0.1";
            std::string default_port = "8080";
            char *default_args[] = {argv[0], const_cast<char *>(default_username.c_str()),
                                    const_cast<char *>(default_host.c_str()), const_cast<char *>(default_port.c_str())};
            argc = sizeof(default_args) / sizeof(default_args[0]);
            argv = default_args;
            std::cout
                    << "WARN: Wrong expected inputs, connection might not be guaranteed. Using default values.\nProper usage: chat_client <username> <host> <port>\n";
            std::cout << "Will attempt connection to " << argv[2] << ":" << argv[3] << " as " << argv[1] << std::endl;
            connectToServer(argv);
        } else if (argc > 4) { //Fail if more inputs that expected
            std::cerr << "ERR:  Wrong expected inputs, proper usage: chat_client <username> <host> <port>\n";
            return 1;
        } else {
            connectToServer(argv);
        }
    } catch (std::exception &e) {
        std::cerr << "ERR:  Exception: " << e.what() << "\n";
    }

    std::cout << "Press any key to exit...";
    system("pause");
    return 0;
}

