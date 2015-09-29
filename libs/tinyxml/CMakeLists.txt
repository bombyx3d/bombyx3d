
project(tinyxml)
include(../../cmake/DisableWarnings.cmake)

file(GLOB files *.cpp *.h)
source_group("Source Files" FILES ${files})

z_add_library(tinyxml STATIC EXCLUDE_FROM_ALL ${files})
target_include_directories("${tinyxml}" SYSTEM BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
