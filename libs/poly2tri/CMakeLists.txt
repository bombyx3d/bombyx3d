
project(poly2tri)
include(../../cmake/DisableWarnings.cmake)

file(GLOB_RECURSE source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" poly2tri/*)
z_set_source_groups(${source_files})

z_add_library(poly2tri STATIC EXCLUDE_FROM_ALL ${source_files})

target_include_directories("${poly2tri}" SYSTEM BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
