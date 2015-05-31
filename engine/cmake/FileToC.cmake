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

if(NOT __Z_FILE_TO_C_CMAKE_INCLUDED)
    set(__Z_FILE_TO_C_CMAKE_INCLUDED TRUE)

    include(CMakeParseArguments)

    get_filename_component(_Z_FILE_TO_C_PATH "${CMAKE_CURRENT_LIST_FILE}" PATH)
    get_filename_component(_Z_FILE_TO_C_PATH "${_Z_FILE_TO_C_PATH}" ABSOLUTE)

    macro(z_file_to_c)

        set(options)
        set(oneValueArgs INPUT OUTPUT SYMBOL NAMESPACE)
        set(multiValueArgs)
        cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

        get_filename_component(input "${ARGS_INPUT}" ABSOLUTE)
        get_filename_component(output "${ARGS_OUTPUT}" ABSOLUTE)

        add_custom_command(OUTPUT
                "${ARGS_OUTPUT}"
            COMMAND
                "${CMAKE_COMMAND}"
                    -Dinput="${input}"
                    -Doutput="${output}"
                    -Dsymbol="${ARGS_SYMBOL}"
                    -Dnamespace="${ARGS_NAMESPACE}"
                    -D_Z_FILE_TO_C_IMPL:BOOL=YES
                    -P "${_Z_FILE_TO_C_PATH}/FileToC.cmake"
            DEPENDS
                "${ARGS_INPUT}"
                "${_Z_FILE_TO_C_PATH}/FileToC.cmake"
            MAIN_DEPENDENCY
                "${ARGS_INPUT}"
            WORKING_DIRECTORY
                "${CMAKE_CURRENT_BINARY_DIR}"
        )

    endmacro()

endif()

if(_Z_FILE_TO_C_IMPL)

    file(READ "${input}" contents HEX)
    string(LENGTH "${contents}" contents_length)

    set(c_file "#pragma once\n")
    set(c_file "${c_file}#include <stdint.h>\n")
    if(NOT "${namespace}" STREQUAL "")
        set(c_file "${c_file}namespace ${namespace} {\n")
    endif()
    set(c_file "${c_file}extern const size_t ${symbol}_size;\n")
    set(c_file "${c_file}extern const uint8_t ${symbol}[];\n")
    set(c_file "${c_file}#ifndef Z_FILE2C_HEADER_ONLY\n")
    set(c_file "${c_file}const uint8_t ${symbol}[] = {\n")

    set(offset 0)
    set(counter 0)
    while(${offset} LESS ${contents_length})
        string(SUBSTRING ${contents} ${offset} 2 code)
        set(c_file "${c_file}0x${code},")
        math(EXPR counter "${counter} + 1")
        if(${counter} EQUAL 32)
            set(c_file "${c_file}\n")
            set(counter 0)
        endif()
        math(EXPR offset "${offset} + 2")
    endwhile()

    if(NOT ${counter} EQUAL 0)
        set(c_file "${c_file}\n")
    endif()
    set(c_file "${c_file}};\n")

    set(c_file "${c_file}const size_t ${symbol}_size = ${offset};\n")
    set(c_file "${c_file}#endif /* Z_FILE2C_HEADER_ONLY */\n")
    if(NOT "${namespace}" STREQUAL "")
        set(c_file "${c_file}}\n")
    endif()

    file(WRITE "${output}" "${c_file}")

endif()
