//
// Created by casta on 06/04/2023.
//

#include "gtest/gtest.h"

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

TEST_F(FIXTURE_NAME, Dummy) {
    SUCCEED();
}
