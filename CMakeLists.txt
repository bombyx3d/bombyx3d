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
project(Engine)

set(__Z_ENGINE_DONT_ADD_SUBDIRECTORY TRUE)
include(cmake/Engine.cmake)

add_subdirectory(3rdparty)

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
    canvas/*.cpp
    canvas/*.h
    cmake/*.cmake
    core/interfaces/*.cpp
    core/interfaces/*.h
    core/io/files/*.cpp
    core/io/files/*.h
    core/io/streams/*.cpp
    core/io/streams/*.h
    core/math/*.cpp
    core/math/*.h
    core/*.h
    io/zip/*.cpp
    io/zip/*.h
    io/*.cpp
    io/*.h
    mechanics/match3/*.cpp
    mechanics/match3/*.h
    renderer/formats/*.h
    renderer/opengl/api/dummy/*.h
    renderer/opengl/api/normal/*.h
    renderer/opengl/api/qt5/*.h
    renderer/opengl/api/*.cpp
    renderer/opengl/api/*.h
    renderer/opengl/*.cpp
    renderer/opengl/*.h
    renderer/*.cpp
    renderer/*.h
    utility/*.cpp
    utility/*.h
    *.cpp
    *.h
)

file(GLOB shader_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    renderer/shaders/*.glsl
)

z_set_source_groups(${glm_files})
z_set_source_groups(${source_files})
z_set_source_groups(${shader_files})

set_source_files_properties(${glm_files} PROPERTIES HEADER_FILE_ONLY TRUE)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/shaders")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
foreach(shader_file ${shader_files})
    get_filename_component(output_file "${shader_file}" NAME)
    string(MAKE_C_IDENTIFIER "${output_file}" symbol_name)
    set(output_file "${CMAKE_CURRENT_BINARY_DIR}/shaders/${output_file}.h")
    set_source_files_properties("${output_file}" PROPERTIES GENERATED TRUE)
    z_file_to_c(OUTPUT "${output_file}" INPUT "${shader_file}" SYMBOL "${symbol_name}" NAMESPACE Z)
    list(APPEND source_files "${output_file}")
    source_group("Generated Files" FILES "${output_file}")
endforeach()

add_library(libengine STATIC ${source_files} ${shader_files})

set_target_properties(libengine PROPERTIES OUTPUT_NAME "engine" CXX_STANDARD 11)
target_link_libraries(libengine libjpeg libglew libunzip libyaml)
z_target_link_qt5(libengine)
z_target_link_opengl(libengine)

add_subdirectory(platform)
target_link_libraries(libengine libplatform)

add_subdirectory(doc)
