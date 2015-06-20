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
project(Engine)

set(__Z_ENGINE_DONT_ADD_SUBDIRECTORY TRUE)
include(build/cmake/Engine.cmake)

set(Z_BUILD_DOCUMENTATION NO CACHE STRING "Build engine documentation")
set(Z_BUILD_SAMPLES NO CACHE STRING "Build engine samples")

add_subdirectory(3rdparty)
add_subdirectory(_OLD)
add_subdirectory(core)
add_subdirectory(modules)
add_subdirectory(system)

if (Z_BUILD_DOCUMENTATION)
    if(NOT WIN32)
        find_package(Doxygen)
    else()
        set(DOXYGEN_EXECUTABLE "${Engine_SOURCE_DIR}/tools/win32/doxygen.cmd")
        set(DOXYGEN_FOUND TRUE)
        set(DOXYGEN_VERSION "1.8.8")
        set(DOXYGEN_DOT_EXECUTABLE "${Engine_SOURCE_DIR}/tools/win32/dot.exe")
        set(DOXYGEN_DOT_FOUND TRUE)
        set(DOXYGEN_DOT_PATH "${Engine_SOURCE_DIR}/tools/win32")
    endif()

    set(doc_source_files
        "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile"
        "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
        "${EngineCore_SOURCE_DIR}/doc/Logging.dox"
        "${EngineCore_SOURCE_DIR}/doc/Textures.dox"
    )

    if(DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)
        add_custom_target(doc ALL
            COMMAND
                "${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile"
            DEPENDS
                ${doc_source_files}
            WORKING_DIRECTORY
                "${CMAKE_CURRENT_SOURCE_DIR}"
            COMMENT
                "Generating documentation"
            SOURCES
                ${doc_source_files}
        )
    endif()
endif()

if(Z_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()
