cmake_minimum_required(VERSION 3.5)

project(mawk)

find_package(Boost 1.72.0 REQUIRED COMPONENTS
                program_options)

include_directories(${Boost_INCLUDE_DIRS})

add_library(mawk_core parser.cpp)

add_executable(mawk
    main.cpp
    )

target_link_libraries(mawk
        Boost::program_options)


