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
project(glew C)

file(GLOB source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" include/GL/*.h src/*.c)
z_set_source_groups(${source_files})
add_library(libglew STATIC EXCLUDE_FROM_ALL ${source_files})

if(Z_OPENGL_FOUND)
    set_source_files_properties(src/dummy.c PROPERTIES HEADER_FILE_ONLY TRUE)
else()
    set_source_files_properties(src/glew.c PROPERTIES HEADER_FILE_ONLY TRUE)
endif()

set_target_properties(libglew PROPERTIES OUTPUT_NAME "glew")
target_include_directories(libglew PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_definitions(libglew PRIVATE GLEW_BUILD)
target_compile_definitions(libglew PUBLIC GLEW_STATIC GLEW_NO_GLU)
z_target_link_opengl(libglew)
