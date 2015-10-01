
project(angle)
include(../../cmake/DisableWarnings.cmake)

add_definitions("-DANGLE_STANDALONE_BUILD")
if(B3D_WINPHONE OR B3D_WINSTORE)
    add_definitions("-DANGLE_ENABLE_D3D11")
elseif(B3D_WIN32)
    add_definitions("-DANGLE_ENABLE_D3D11" "-DANGLE_ENABLE_D3D9")
endif()

add_definitions("-DGL_APICALL=" "-DEGLAPI=" "-DGL_GLEXT_PROTOTYPES=" "-DNOMINMAX")

include(FileList.cmake)
b3d_set_source_groups(${source_files})
b3d_add_library(angle STATIC EXCLUDE_FROM_ALL ${source_files})

target_include_directories("${angle}" SYSTEM BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories("${angle}" BEFORE PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
