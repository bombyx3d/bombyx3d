
project(boost)

file(GLOB_RECURSE source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" boost/*)
set_source_groups(${source_files})

add_library(boost INTERFACE EXCLUDE_FROM_ALL ${source_files})

target_include_directories(boost SYSTEM BEFORE INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
