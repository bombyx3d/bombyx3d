
project(cvtutf)
include(../../cmake/DisableWarnings.cmake)

set(source_files ConvertUTF.c ConvertUTF.h)
b3d_set_source_groups(${source_files})

b3d_add_library(cvtutf STATIC EXCLUDE_FROM_ALL ${source_files})

target_include_directories("${cvtutf}" SYSTEM BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
