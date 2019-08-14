cmake_minimum_required(VERSION 3.10)
project(mawk)

include_directories(
        /home/bhashitha/Downloads/boost_1_70_0/
        /usr/local/include/ 
        )

link_directories(
    /home/bhashitha/Downloads/boost_1_70_0/stage/lib
    /usr/local/lib/ 
    )

add_executable(mawk_test
    main.cpp)

target_link_libraries(mawk_test
        boost_program_options
        mawk_core
        gtest
        gmock
        pthread
        )

