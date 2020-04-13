cmake_minimum_required(VERSION 3.5)
project(mawk)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

find_package(Boost 1.72.0 REQUIRED COMPONENTS
                program_options)

include_directories(/usr/local/include/)

link_directories( /usr/local/lib/ )

add_executable(mawk_test
    main.cpp)

target_link_libraries(mawk_test
        Boost::program_options
        mawk_core
        gtest
        gmock
        pthread
        )

