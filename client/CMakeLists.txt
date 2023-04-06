cmake_minimum_required(VERSION 3.23)
project(ClientApp)

set(CMAKE_CXX_STANDARD 20)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.81.0 REQUIRED)
find_package(Boost COMPONENTS thread)

set(client_app_SRCS client_app.cpp)

add_executable(${PROJECT_NAME} ${client_app_SRCS})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${Boost_INCLUDE_DIRS}
        PRIVATE
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${Boost_LIBRARIES}
        )