
project(bzip2 C)

file(GLOB root_files *.c *.h)
source_group("Source Files" FILES ${root_files})

add_library(libbzip2 STATIC EXCLUDE_FROM_ALL
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

set_target_properties(libbzip2 PROPERTIES OUTPUT_NAME "bzip2")
target_include_directories(libbzip2 BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(bzip2 EXCLUDE_FROM_ALL bzip2.c)
target_link_libraries(bzip2 libbzip2)
