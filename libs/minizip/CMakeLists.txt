
project(minizip C)
include(../../cmake/DisableWarnings.cmake)

add_definitions(-DHAVE_BZIP2)

set(libunzip_source_files
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

set(libzip_source_files
    minizip.c
    zip.c
    zip.h
)

source_group("Source Files" FILES ${libzip_source_files} ${libunzip_source_files})

# libunzip

add_library(libunzip STATIC EXCLUDE_FROM_ALL ${libunzip_source_files})

if(NOT WIN32)
    set_source_files_properties(iowin32.c PROPERTIES HEADER_FILE_ONLY TRUE)
endif()

set_target_properties(libunzip PROPERTIES OUTPUT_NAME "unzip")
target_include_directories(libunzip BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(libunzip zlib libbzip2)

# libzip

# add_library(libzip STATIC EXCLUDE_FROM_ALL ${libzip_source_files})

# set_target_properties(libzip PROPERTIES OUTPUT_NAME "zip")
# target_include_directories(libzip BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
# target_compile_definitions(libzip PRIVATE LIBZIP)
# target_link_libraries(libzip zlib libbzip2 libunzip)

# executables

# add_executable(minizip EXCLUDE_FROM_ALL minizip.c)
# target_link_libraries(minizip libzip)

# add_executable(miniunz EXCLUDE_FROM_ALL miniunz.c)
# target_link_libraries(miniunz libunzip)
