
project(glm)
include(../../cmake/DisableWarnings.cmake)

file(GLOB_RECURSE source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" glm/*)
b3d_set_source_groups(${source_files})

b3d_add_library(glm STATIC EXCLUDE_FROM_ALL ${source_files})

target_include_directories("${glm}" SYSTEM BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
