#
# Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

cmake_minimum_required(VERSION 3.2)
project(assimp)

z_disable_warnings()

set(GIT_BRANCH "master")
set(GIT_COMMIT_HASH 0)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/revision.h.in" "${CMAKE_CURRENT_BINARY_DIR}/revision.h")

include_directories(code/BoostWorkaround)
add_definitions(-DASSIMP_BUILD_BOOST_WORKAROUND)
add_definitions(-DASSIMP_BUILD_NO_OWN_ZLIB)

set(ASSIMP_NO_EXPORT OFF CACHE BOOL "Disable Assimp's export functionality.")
if(ASSIMP_NO_EXPORT)
    add_definitions(-DASSIMP_BUILD_NO_EXPORT)
endif(ASSIMP_NO_EXPORT)

file(GLOB source_files
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    code/*.cpp
    code/*.h
    code/BoostWorkaround/boost/math/*.hpp
    code/BoostWorkaround/boost/tuple/*.hpp
    code/BoostWorkaround/boost/*.hpp
    contrib/ConvertUTF/*.c contrib/ConvertUTF/*.h
    contrib/clipper/*.cpp contrib/clipper/*.hpp
    contrib/irrXML/*.cpp contrib/irrXML/*.h
    contrib/poly2tri/poly2tri/common/*.cc
    contrib/poly2tri/poly2tri/common/*.h
    contrib/poly2tri/poly2tri/sweep/*.cc
    contrib/poly2tri/poly2tri/sweep/*.h
    contrib/poly2tri/poly2tri/*.h
)

file(GLOB code_inl_files code/*.inl)
set_source_files_properties(${code_inl_files} PROPERTIES HEADER_FILE_ONLY TRUE)

z_set_source_groups(${source_files} ${code_inl_files})

add_library(libassimp STATIC
    ${source_files}
    ${code_inl_files}
)

target_include_directories(libassimp BEFORE PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" include)
target_link_libraries(libassimp libunzip libz)
set_target_properties(libassimp PROPERTIES OUTPUT_NAME "assimp")

add_subdirectory(tools/assimp_cmd)
