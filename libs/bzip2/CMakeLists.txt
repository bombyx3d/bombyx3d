
project(bzip2 C)
include(../../cmake/DisableWarnings.cmake)

file(GLOB root_files *.c *.h)
source_group("Source Files" FILES ${root_files})

z_add_library(bzip2 STATIC EXCLUDE_FROM_ALL
    blocksort.c
    bzlib.c
    bzlib.h
    bzlib_private.h
    compress.c
    crctable.c
    decompress.c
    huffman.c
    randtable.c
)

set_target_properties("${bzip2}" PROPERTIES OUTPUT_NAME "bzip2")
target_include_directories("${bzip2}" BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
