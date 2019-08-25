cmake_minimum_required(VERSION 3.5)
project(mawk)

include_directories(/home/bhashitha/Downloads/boost_1_70_0/)

link_directories(/home/bhashitha/Downloads/boost_1_70_0/stage/lib)

add_library(mawk_core parser.cpp)

add_executable(mawk
    main.cpp
    )

target_link_libraries(mawk
        boost_program_options)


