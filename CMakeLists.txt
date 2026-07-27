cmake_minimum_required(VERSION 3.20)

cmake_policy(SET CMP0144 NEW)

project(timestamp
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "Print the latest modification time across all files in a directory"
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(Boost REQUIRED COMPONENTS program_options regex)

add_executable(timestamp src/timestamp.cpp)

target_link_libraries(timestamp PRIVATE
    Boost::program_options
    Boost::regex
)

install(TARGETS timestamp RUNTIME DESTINATION bin)
