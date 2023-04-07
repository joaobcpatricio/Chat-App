//
// Created by casta on 06/04/2023.
//

#include "gtest/gtest.h"
#include <boost/asio.hpp>

#include "client_app.hpp"
#include "server_app.hpp"

#undef FAIL
#define FAIL_TEST()     GTEST_FAIL()

#define FIXTURE_NAME    ExampleClassTest



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


