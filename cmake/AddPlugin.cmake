#
# Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com).
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

include(CMakeParseArguments)

macro(b3d_add_plugin name)

    string(REPLACE "/" "-" target "bombyx3d-plugin-${name}")

    set(options)
    set(oneValueArgs CXX_STANDARD)
    set(multiValueArgs SOURCES LIBRARIES)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT ARGS_CXX_STANDARD)
        set(ARGS_CXX_STANDARD 11)
    endif()

    b3d_set_source_groups(${ARGS_SOURCES})
    add_library("${target}" STATIC EXCLUDE_FROM_ALL ${ARGS_SOURCES})

    set_target_properties("${target}" PROPERTIES CXX_STANDARD "${ARGS_CXX_STANDARD}" FOLDER "Bombyx3D")
    target_link_libraries("${target}" bombyx3d-core)

    get_filename_component(path "${CMAKE_CURRENT_SOURCE_DIR}/__init__.cpp" ABSOLUTE)
    if(EXISTS "${path}")
        if(WIN32)
            string(REPLACE "\\" "/" path "${path}")
        endif()
        set_property(TARGET "${target}" APPEND PROPERTY BOMBYX3D_INITIALIZERS "${path}")
    endif()

    foreach(library ${ARGS_LIBRARIES})
        b3d_target_link_library("${target}" "${library}")
    endforeach()

endmacro()
