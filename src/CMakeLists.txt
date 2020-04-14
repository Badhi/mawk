cmake_minimum_required(VERSION 3.5)

project(mawk)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

find_package(Boost 1.72.0 REQUIRED COMPONENTS
                program_options)

if (COVERAGE)
    message(STATUS "Enabling Coverage")
    set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")
endif()

include_directories(${Boost_INCLUDE_DIRS})

add_library(mawk_core 
        parser.cpp
        RunArgs.cpp)

add_executable(mawk
    main.cpp
    )

target_link_libraries(mawk
        Boost::program_options
        mawk_core)


