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

get_filename_component(B3D_CMAKE_SCRIPTS_PATH "${CMAKE_CURRENT_LIST_FILE}" ABSOLUTE)
get_filename_component(B3D_CMAKE_SCRIPTS_PATH "${B3D_CMAKE_SCRIPTS_PATH}" PATH)
get_filename_component(B3D_ENGINE_PATH "${B3D_CMAKE_SCRIPTS_PATH}" PATH)

macro(_b3d_link_plugin_init target plugin_name)
    get_property(initializers TARGET "${plugin_name}" PROPERTY BOMBYX3D_INITIALIZERS)
    if(initializers)
        set_property(TARGET "${target}" APPEND PROPERTY BOMBYX3D_INITIALIZERS ${initializers})
    endif()
endmacro()

macro(b3d_target_link_library target name)
    string(REPLACE "/" "-" library_name "bombyx3d-support-${name}")
    string(REPLACE "/" "-" plugin_name "bombyx3d-plugin-${name}")
    if(TARGET "${library_name}")
        target_link_libraries("${target}" "${library_name}")
    elseif(TARGET "${plugin_name}")
        target_link_libraries("${target}" "${plugin_name}")
        _b3d_link_plugin_init("${target}" "${plugin_name}")
    elseif(EXISTS "${B3D_ENGINE_PATH}/libs/${name}/CMakeLists.txt")
        message(STATUS "Adding library: ${name}")
        file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bombyx3d/libs/${name}")
        add_subdirectory("${B3D_ENGINE_PATH}/libs/${name}" "${CMAKE_BINARY_DIR}/bombyx3d/libs/${name}")
        target_link_libraries("${target}" "${library_name}")
    elseif(EXISTS "${B3D_ENGINE_PATH}/plugins/${name}/CMakeLists.txt")
        message(STATUS "Adding plugin: ${name}")
        file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bombyx3d/plugins/${name}")
        add_subdirectory("${B3D_ENGINE_PATH}/plugins/${name}" "${CMAKE_BINARY_DIR}/bombyx3d/plugins/${name}")
        target_link_libraries("${target}" "${plugin_name}")
        _b3d_link_plugin_init("${target}" "${plugin_name}")
    else()
        target_link_libraries("${target}" "${name}")
    endif()
endmacro()

macro(b3d_target_link_libraries target)
    foreach(library ${ARGN})
        b3d_target_link_library("${target}" "${library}")
    endforeach()
endmacro()
