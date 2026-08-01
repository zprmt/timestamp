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

enable_testing()

# Boost is linked statically by default. With shared Boost on MSVC, the
# program_options headers get BOOST_PROGRAM_OPTIONS_DYN_LINK and declare
# variables_map as __declspec(dllimport); MSVC propagates that onto inherited
# std::map members (e.g. contains()), which then fail to link because the Boost
# DLL doesn't export them. Static Boost removes the DLL entirely.
option(TIMESTAMP_STATIC_BOOST "Link Boost statically (avoids MSVC dllimport issues)" ON)
if(TIMESTAMP_STATIC_BOOST)
    # Read by find_package(Boost) below - must be set BEFORE the call.
    set(Boost_USE_STATIC_LIBS ON)
endif()

find_package(Boost REQUIRED COMPONENTS program_options regex)

add_executable(timestamp src/timestamp.cpp)

target_link_libraries(timestamp PRIVATE
    Boost::program_options
    Boost::regex
)

install(TARGETS timestamp RUNTIME DESTINATION bin)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    add_test(NAME smoke
        COMMAND ${CMAKE_SOURCE_DIR}/cmake/tests/smoke.sh
            $<TARGET_FILE:timestamp>
            ${CMAKE_SOURCE_DIR}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    add_test(NAME smoke
        COMMAND ${CMAKE_SOURCE_DIR}/cmake/tests/smoke.sh
            $<TARGET_FILE:timestamp>
            ${CMAKE_SOURCE_DIR}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    add_test(NAME smoke
        COMMAND powershell -ExecutionPolicy Bypass -File
            ${CMAKE_SOURCE_DIR}/cmake/tests/smoke.ps1
            -Bin $<TARGET_FILE:timestamp>
            -Src ${CMAKE_SOURCE_DIR}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()
