
project(libpng C)
include(../../cmake/DisableWarnings.cmake)

configure_file(scripts/pnglibconf.h.prebuilt "${CMAKE_CURRENT_BINARY_DIR}/pnglibconf.h" COPYONLY)

set(scripts_files scripts/pnglibconf.h.prebuilt)
file(GLOB root_files *.c *.h)

z_add_library(libpng STATIC EXCLUDE_FROM_ALL
    ${script_files}
    ${root_files}
)

source_group("Source Files\\scripts" FILES ${scripts_files})
source_group("Source Files" FILES ${root_files})

set_source_files_properties(scripts/pnglibconf.h.prebuilt PROPERTIES HEADER_FILE_ONLY TRUE)
set_target_properties("${libpng}" PROPERTIES OUTPUT_NAME "png")
target_include_directories("${libpng}" BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories("${libpng}" BEFORE PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
z_target_link_libraries("${libpng}" zlib)
