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

cmake_minimum_required(VERSION 3.1)
project(minizip C)

if(MSVC)
    add_definitions(/wd4101 /wd4244 /wd4267 /wd4456)
endif()

add_definitions(-DHAVE_BZIP2)

set(source_files
    crypt.h
    ioapi.c
    ioapi.h
    ioapi_buf.c
    ioapi_buf.h
    ioapi_mem.c
    ioapi_mem.h
    iowin32.c
    iowin32.h
    unzip.c
    unzip.h
    zip.c
    zip.h
)

source_group("Source Files" FILES ${source_files})

add_library(libminizip STATIC ${source_files})

if(NOT WIN32)
    set_source_files_properties(iowin32.c PROPERTIES HEADER_FILE_ONLY TRUE)
endif()

set_target_properties(libminizip PROPERTIES OUTPUT_NAME "minizip")
target_include_directories(libminizip BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(libminizip zlib bzip2)

add_executable(minizip EXCLUDE_FROM_ALL minizip.c)
target_link_libraries(minizip libminizip)
