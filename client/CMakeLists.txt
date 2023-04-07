cmake_minimum_required(VERSION 3.23)
project(ClientApp)

set(CMAKE_CXX_STANDARD 20)

SET(CMAKE_INCLUDE_PATH${CMAKE_INCLUDE_PATH} "C:/local/boost_1_81_0")
SET(CMAKE_LIBRARY_PATH${CMAKE_LIBRARY_PATH} "C:/local/boost_1_81_0/libs")
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.81.0 REQUIRED)
find_package(Boost COMPONENTS thread)


file(GLOB_RECURSE SOURCES src/*.cpp )
set(client_app_SRCS ${SOURCES})

add_executable(${PROJECT_NAME} ${client_app_SRCS})


target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${Boost_INCLUDE_DIRS}
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${Boost_LIBRARIES}
        )

##lib
#add_library(example_class STATIC ${client_app_SRCS})
#
#target_include_directories(example_class
#        PUBLIC
#        $<INSTALL_INTERFACE:include>
#        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#        ${Boost_INCLUDE_DIRS}
#        )
#
#target_link_libraries(example_class
#        PRIVATE
#        ${Boost_LIBRARIES}
#        )