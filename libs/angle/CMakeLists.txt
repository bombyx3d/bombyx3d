
project(angle)
include(../../cmake/DisableWarnings.cmake)

add_definitions(
    "-DANGLE_ENABLE_HLSL"
    "-DANGLE_STANDALONE_BUILD"
    "-DGL_APICALL="
    "-DEGLAPI="
    "-DGL_GLEXT_PROTOTYPES="
    "-DNOMINMAX"
)

if(B3D_WINPHONE OR B3D_WINSTORE)
    add_definitions("-DANGLE_ENABLE_D3D11")
elseif(B3D_WIN32)
    add_definitions("-DANGLE_ENABLE_D3D11" "-DANGLE_ENABLE_D3D9")
endif()

include(FileList.cmake)
b3d_set_source_groups(${source_files})
b3d_add_library(angle STATIC EXCLUDE_FROM_ALL ${source_files})

target_compile_definitions("${angle}" PUBLIC "ANGLE_TRANSLATOR_STATIC")

target_include_directories("${angle}" SYSTEM BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories("${angle}" BEFORE PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

if(WIN32)
    target_link_libraries("${angle}" d3d9 dxguid)
endif()
