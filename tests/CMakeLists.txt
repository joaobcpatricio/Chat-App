project(Google_tests)

SET(CMAKE_INCLUDE_PATH${CMAKE_INCLUDE_PATH} "C:/local/boost_1_81_0")
SET(CMAKE_LIBRARY_PATH${CMAKE_LIBRARY_PATH} "C:/local/boost_1_81_0/libs")
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.81.0 REQUIRED)
find_package(Boost COMPONENTS thread)


add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/client/include)
include_directories(${CMAKE_SOURCE_DIR}/client/src)
include_directories(${CMAKE_SOURCE_DIR}/server/include)
include_directories(${CMAKE_SOURCE_DIR}/server/src)

# Here go all test source files
set(test_SRCS tests.cpp ../client/src/client_app.cpp ../server/src/server_app.cpp ../server/src/server_user.cpp ../server/src/chat_room.cpp ../server/src/worker_thread.cpp)

add_executable(Google_Tests_run ${test_SRCS})
target_link_libraries(Google_Tests_run gtest gmock gtest_main ${Boost_LIBRARIES})

file(GLOB_RECURSE CLIENT_APP_SOURCES ${CMAKE_SOURCE_DIR}/client/src/*.cpp ${CMAKE_SOURCE_DIR}/server/src/*.cpp)

target_include_directories(Google_Tests_run
        PUBLIC
        ${Boost_INCLUDE_DIRS}
        )

