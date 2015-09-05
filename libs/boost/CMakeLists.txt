
project(boost)

file(GLOB_RECURSE source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" boost/*)
list(APPEND source_files boost.cpp)
set_source_groups(${source_files})

add_library(boost STATIC EXCLUDE_FROM_ALL ${source_files})

target_include_directories(boost SYSTEM BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
