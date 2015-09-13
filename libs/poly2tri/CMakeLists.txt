
project(poly2tri)

file(GLOB_RECURSE source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" poly2tri/*)
set_source_groups(${source_files})

add_library(poly2tri STATIC EXCLUDE_FROM_ALL ${source_files})

target_include_directories(poly2tri SYSTEM BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
