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
project(libyaml C)

include_directories(win32)
add_definitions(-DHAVE_CONFIG_H)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    add_definitions(/wd4244 /wd4267 /wd4456 /wd4457)
endif()

file(GLOB include_files include/*)
file(GLOB src_files src/*)
file(GLOB win32_files win32/*)

add_library(libyaml STATIC ${EXCLUDE_FROM_ALL}
    ${include_files}
    ${src_files}
    ${win32_files}
)

source_group("Source Files\\include" FILES ${include_files})
source_group("Source Files\\src" FILES ${src_files})
source_group("Source Files\\win32" FILES ${win32_files})

set_target_properties(libyaml PROPERTIES OUTPUT_NAME "yaml")
target_compile_definitions(libyaml PUBLIC "YAML_DECLARE_STATIC")
target_include_directories(libyaml PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
