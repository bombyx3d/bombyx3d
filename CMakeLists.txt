
cmake_minimum_required(VERSION 3.2)
project(GnomeFortress)

find_package(Threads REQUIRED)

add_subdirectory(cmake)
add_subdirectory(libs)

if(NOT MSVC)
    add_definitions(-pedantic -Wall -Wextra -Wconversion)
    if(NOT CMAKE_COMPILER_IS_GNUCC AND NOT CMAKE_COMPILER_IS_GNUCC)
        add_definitions(-Wshadow)
    endif()
endif()
