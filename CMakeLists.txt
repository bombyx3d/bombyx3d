
cmake_minimum_required(VERSION 3.2)
project(GnomeFortress)

include(cmake/SetSourceGroups.cmake)

find_package(Threads REQUIRED)

add_subdirectory(libs)

if(NOT MSVC)
    add_definitions(-pedantic -Wall -Wextra -Wconversion)
    if(NOT CMAKE_COMPILER_IS_GNUCC AND NOT CMAKE_COMPILER_IS_GNUCC)
        add_definitions(-Wshadow)
    endif()
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(engine)
add_subdirectory(game)

add_executable(GnomeFortress main.cpp)
set_target_properties(GnomeFortress PROPERTIES CXX_STANDARD 11)
target_link_libraries(GnomeFortress game engine)
