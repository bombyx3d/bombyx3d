
project(assimp)
include(../../cmake/DisableWarnings.cmake)

set(GIT_BRANCH "master")
set(GIT_COMMIT_HASH 0)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/revision.h.in" "${CMAKE_CURRENT_BINARY_DIR}/revision.h")

include_directories(code/BoostWorkaround)
add_definitions(-DASSIMP_BUILD_BOOST_WORKAROUND)
add_definitions(-DASSIMP_BUILD_NO_OWN_ZLIB)
add_definitions(-DASSIMP_BUILD_NO_EXPORT)

file(GLOB source_files
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    code/*.cpp
    code/*.h
    code/BoostWorkaround/boost/math/*.hpp
    code/BoostWorkaround/boost/tuple/*.hpp
    code/BoostWorkaround/boost/*.hpp
    contrib/clipper/*.cpp
    contrib/clipper/*.hpp
)

file(GLOB code_inl_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" code/*.inl)
set_source_files_properties(${code_inl_files} PROPERTIES HEADER_FILE_ONLY TRUE)

b3d_set_source_groups(${source_files} ${code_inl_files})
b3d_add_library(assimp STATIC EXCLUDE_FROM_ALL ${source_files} ${code_inl_files})
b3d_target_link_libraries("${assimp}" cvtutf irrxml poly2tri minizip zlib)

set_target_properties("${assimp}" PROPERTIES
    CXX_STANDARD 11
    COTIRE_C_PREFIX_HEADER_INIT "code/AssimpPCH.h"
    COTIRE_CXX_PREFIX_HEADER_INIT "code/AssimpPCH.h"
    COTIRE_ENABLE_PRECOMPILED_HEADER TRUE
    COTIRE_ADD_UNITY_BUILD FALSE
)

target_include_directories("${assimp}" SYSTEM BEFORE PUBLIC
    "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/include")

cotire("${assimp}")
