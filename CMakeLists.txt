cmake_minimum_required(VERSION 3.23)
project(ChatApp)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)

add_subdirectory(tests)