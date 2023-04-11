//
// Created by casta on 06/04/2023.
//

#include "gtest/gtest.h"
#include <boost/asio.hpp>
#include <chrono>
#include <ctime>

#include "client_app.hpp"
#include "server_app.hpp"
#include "chat_room.hpp"
#include "worker_thread.h"

#undef FAIL
#define FAIL_TEST()     GTEST_FAIL()

#define FIXTURE_NAME    ChatAppTests


class FIXTURE_NAME : public ::testing::Test {
protected:

    void FixtureSetUp() {
    }

    void FixtureTearDown() {
        // The teardown code
    }


// Wrap the methods to the class-methods
    void SetUp() override {
        FixtureSetUp();
    }

    void TearDown() override {
        FixtureTearDown();
    }
};


/// Here go the tests --------------------------------------------------------------------------------------------------

// -- Tests for the Client App --
TEST_F(FIXTURE_NAME, ClientCreatAppWithUserNameNoExceptionsFromSubCalls) {
    boost::asio::io_service io_service;
    tcp::resolver::query query("127.0.0.1", "8080");
    tcp::resolver resolver(io_service);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    std::array<char, MAX_USERNAME> username = {'u', 's', 'e', 'r', 'n', 'a', 'm', 'e'};
    clientApp app(username, io_service, endpoint_iterator);

    std::array<char, MAX_USERNAME> app_usrname = app.getUsername();
    EXPECT_EQ(app_usrname, username);
}

TEST_F(FIXTURE_NAME, ClentAssertWriteDoesNotThrowException) {
    boost::asio::io_service io_service;
    tcp::resolver::query query("127.0.0.1", "8080");
    tcp::resolver resolver(io_service);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    std::array<char, MAX_USERNAME> username = {'u', 's', 'e', 'r', 'n', 'a', 'm', 'e'};
    clientApp app(username, io_service, endpoint_iterator);

    std::array<char, MAX_IP_PKT_SIZE> msg{'H', 'e', 'l', 'l', 'o'};
    ASSERT_NO_THROW(app.write(msg));
}

TEST_F(FIXTURE_NAME, ClientNoIssuesWithSocketCloseCall) {
    boost::asio::io_service io_service;
    tcp::resolver::query query("127.0.0.1", "8080");
    tcp::resolver resolver(io_service);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    std::array<char, MAX_USERNAME> username = {'u', 's', 'e', 'r', 'n', 'a', 'm', 'e'};
    clientApp app(username, io_service, endpoint_iterator);

    app.close();
    ASSERT_NO_THROW(app.close());
}


// -- Tests for the Server App --

TEST_F(FIXTURE_NAME, WorkerThreadTestTimeFormatt) {
    // Test the timestamp format
    std::string timestamp = worker_space::getTimestamp();
    EXPECT_EQ(timestamp.size(), 22);
    EXPECT_EQ(timestamp[0], '[');
    EXPECT_EQ(timestamp[20], ']');
    EXPECT_EQ(timestamp[5], '-');
    EXPECT_EQ(timestamp[11], ' ');
    EXPECT_EQ(timestamp[14], ':');

    // Test that the timestamp is within a reasonable range
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&t);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
    std::string expected_timestamp = "[" + std::string(buffer) + "] ";
    EXPECT_EQ(timestamp, expected_timestamp); // The timestamp should match the expected value
}

TEST_F(FIXTURE_NAME, WorkerThreadTestRun) {
    std::shared_ptr<boost::asio::io_service> io_service = std::make_shared<boost::asio::io_service>();
    worker_space::workerThread thread;

    std::ostringstream expected_output;
    expected_output << "[" << std::this_thread::get_id() << "] Thread stared\n";
    expected_output << "[" << std::this_thread::get_id() << "] Thread ended\n";
    testing::internal::CaptureStdout();
    thread.run(io_service);
    std::string actual_output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(actual_output, expected_output.str());
}




