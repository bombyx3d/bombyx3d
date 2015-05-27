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

set(__Z_ENGINE_DONT_ADD_SUBDIRECTORY TRUE)
include(cmake/Engine.cmake)

add_subdirectory(3rdparty)

file(GLOB source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    cmake/*.cmake
    io/files/*.cpp
    io/files/*.h
    io/streams/*.cpp
    io/streams/*.h
    io/zip/*.cpp
    io/zip/*.h
    io/*.cpp
    io/*.h
    opengl/api/dummy/*.h
    opengl/api/normal/*.h
    opengl/api/qt5/*.h
    opengl/api/*.cpp
    opengl/api/*.h
    opengl/*.cpp
    opengl/*.h
    utility/*.cpp
    utility/*.h
    *.cpp
    *.h
)

z_set_source_groups(${source_files})
add_library(libengine STATIC ${source_files})

set_target_properties(libengine PROPERTIES OUTPUT_NAME "engine" CXX_STANDARD 11)
target_link_libraries(libengine libjpeg libglew libunzip)
z_target_link_qt5(libengine)
z_target_link_opengl(libengine)

add_subdirectory(platform)
target_link_libraries(libengine libplatform)
