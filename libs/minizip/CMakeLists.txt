
project(minizip C)
include(../../cmake/DisableWarnings.cmake)

add_definitions(-DHAVE_BZIP2)

set(source_files
    crypt.h
    ioapi.c
    ioapi.h
    ioapi_buf.c
    ioapi_buf.h
    ioapi_mem.c
    ioapi_mem.h
    iowin32.c
    iowin32.h
    unzip.c
    unzip.h
)

source_group("Source Files" FILES ${source_files})
b3d_add_library(minizip STATIC EXCLUDE_FROM_ALL ${source_files})

if(NOT WIN32)
    set_source_files_properties(iowin32.c PROPERTIES HEADER_FILE_ONLY TRUE)
endif()

target_include_directories("${minizip}" BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
b3d_target_link_libraries("${minizip}" zlib bzip2)
