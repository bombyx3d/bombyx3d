
project(pegtl)
include(../../cmake/DisableWarnings.cmake)

file(GLOB_RECURSE source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" pegtl/*)
list(APPEND source_files pegtl.cpp pegtl.hh)
z_set_source_groups(${source_files})

add_library(pegtl STATIC EXCLUDE_FROM_ALL ${source_files})

target_include_directories(pegtl SYSTEM BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
set_target_properties(pegtl PROPERTIES CXX_STANDARD 11)
