cmake_minimum_required(VERSION 3.15)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

# Enable testing
enable_testing()

file(GLOB_RECURSE TEST_FILES "*.cpp")
foreach(file ${TEST_FILES})
    get_filename_component(name ${file} NAME_WE)
    add_executable(${name} ${file})
    target_link_libraries(${name} cppbase GTest::gtest GTest::gtest_main Threads::Threads)
    add_test(NAME ${name} COMMAND ${name})
endforeach()
