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
project(libz C)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

file(GLOB source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.c *.h)
z_set_source_groups(${source_files})
add_library(libz STATIC EXCLUDE_FROM_ALL ${source_files})

set_target_properties(libz PROPERTIES OUTPUT_NAME "z")
target_include_directories(libz BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

include(CheckIncludeFile)
check_include_file(unistd.h HAVE_UNISTD_H)
if(HAVE_UNISTD_H)
    target_compile_definitions(libz PUBLIC "Z_HAVE_UNISTD_H")
endif()

include(CheckTypeSize)
set(CMAKE_REQUIRED_DEFINITIONS "-D_LARGEFILE64_SOURCE=1")
check_type_size(off64_t OFF64_T)
if(HAVE_OFF64_T)
    target_compile_definitions(libz PUBLIC "_LARGEFILE64_SOURCE=1")
endif()
