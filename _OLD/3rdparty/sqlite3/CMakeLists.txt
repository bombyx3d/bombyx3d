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
project(sqlite3 C)

find_package(Threads)

if(MSVC)
    add_definitions(/wd4456 /wd4459)
endif()

file(GLOB source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.c *.h)
z_set_source_groups(${source_files})

add_library(libsqlite3 STATIC EXCLUDE_FROM_ALL sqlite3.c sqlite3.h sqlite3ext.h)
set_target_properties(libsqlite3 PROPERTIES OUTPUT_NAME "sqlite3")
target_include_directories(libsqlite3 BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(libsqlite3 ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})
