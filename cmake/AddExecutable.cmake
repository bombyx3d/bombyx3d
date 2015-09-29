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

macro(_b3d_generate_plugins_initializer target)
    get_property(initializers TARGET "${target}" PROPERTY BOMBYX3D_INITIALIZERS)
    set(string "#undef init\n")
    set(inits)
    set(counter 1)
    foreach(initializer ${initializers})
        set(string "${string}#define init init_plugin_${counter}\n")
        set(string "${string}#include \"${initializer}\"\n")
        set(string "${string}#undef init\n")
        set(inits "${inits}        init_plugin_${counter}();\n")
        math(EXPR counter "${counter} + 1")
    endforeach()
    set(string "${string}namespace Engine {\n")
    set(string "${string}    void init_plugins() {\n")
    set(string "${string}${inits}")
    set(string "${string}    }\n")
    set(string "${string}}\n")

    set(filename "${CMAKE_CURRENT_BINARY_DIR}/bombyx3d-startup.cpp")
    if(NOT EXISTS "${filename}")
        file(WRITE "${filename}" "${string}")
    else()
        file(READ "${filename}" original)
        if(NOT "${string}" STREQUAL "${original}")
            file(WRITE "${filename}" "${string}")
        endif()
    endif()

    source_group("Generated Files" FILES "${filename}")
    target_sources("${target}" PRIVATE "${filename}")
endmacro()

macro(b3d_add_executable name)

    set(options)
    set(oneValueArgs CXX_STANDARD)
    set(multiValueArgs SOURCES LIBRARIES)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT ARGS_CXX_STANDARD)
        set(ARGS_CXX_STANDARD 11)
    endif()

    b3d_set_source_groups(${ARGS_SOURCES})
    add_executable("${name}" ${ARGS_SOURCES})

    set_target_properties("${name}" PROPERTIES CXX_STANDARD "${ARGS_CXX_STANDARD}")
    target_link_libraries("${name}" bombyx3d-core)

    foreach(library ${ARGS_LIBRARIES})
        b3d_target_link_library("${name}" "${library}")
    endforeach()

    _b3d_generate_plugins_initializer("${name}")

endmacro()
