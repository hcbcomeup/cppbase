cmake_minimum_required(VERSION 3.15)

find_package(Threads REQUIRED)

file(GLOB_RECURSE EXAMPLE_FILES "*.cpp")
foreach(file ${EXAMPLE_FILES})
    get_filename_component(name ${file} NAME_WE)
    add_executable(${name} ${file})
    target_link_libraries(${name} cppbase Threads::Threads)
endforeach()
