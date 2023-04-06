project(Google_tests)

add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Here go all test source files
set(test_SRCS tests.cpp)

add_executable(Google_Tests_run ${test_SRCS})

target_link_libraries(Google_Tests_run gtest gmock gtest_main)