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
project(EngineCore)

file(GLOB glm_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    3rdparty/glm/glm/*.hpp
    3rdparty/glm/glm/detail/*.hpp
    3rdparty/glm/glm/detail/*.inl
    3rdparty/glm/glm/gtc/*.hpp
    3rdparty/glm/glm/gtc/*.inl
    3rdparty/glm/glm/gtx/*.hpp
    3rdparty/glm/glm/gtx/*.inl
)

file(GLOB source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    image/*.cpp
    image/*.h
    interfaces/*.cpp
    interfaces/*.h
    io/files/*.cpp
    io/files/*.h
    io/streams/*.cpp
    io/streams/*.h
    io/*.cpp
    io/*.h
    math/*.cpp
    math/*.h
    private/*.cpp
    private/*.h
    resource/*.cpp
    resource/*.h
    utility/*.cpp
    utility/*.h
    common.cpp
)

set_source_files_properties(${glm_files} PROPERTIES HEADER_FILE_ONLY TRUE)

z_set_source_groups(${glm_files})
z_set_source_groups(${source_files})
add_library(libEngineCore STATIC ${glm_files} ${source_files})

target_include_directories(libEngineCore BEFORE PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glm"
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

set_target_properties(libEngineCore PROPERTIES OUTPUT_NAME "EngineCore" CXX_STANDARD 11)
target_link_libraries(libEngineCore libSystem)
