
cmake_minimum_required(VERSION 3.1)
project(jpeglib C)

if(MSVC)
    add_definitions(/wd4267)
endif()

configure_file(jconfig.vc "${CMAKE_CURRENT_BINARY_DIR}/jconfig.h" COPYONLY)
set_source_files_properties(jconfig.vc PROPERTIES HEADER_FILE_ONLY TRUE)

file(GLOB source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.c *.h)
z_set_source_groups(${source_files})
add_library(libjpeg STATIC ${source_files})

set_target_properties(libjpeg PROPERTIES OUTPUT_NAME "jpeg")
target_include_directories(libjpeg BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(libjpeg BEFORE PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
