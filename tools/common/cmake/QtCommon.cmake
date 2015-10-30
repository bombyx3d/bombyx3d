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

if(NOT _B3D_QT_COMMON_INCLUDED)
    set(_B3D_QT_COMMON_INCLUDED TRUE)

    if(POLICY CMP0063)
        cmake_policy(SET CMP0063 NEW)
    endif()

    get_filename_component(B3D_TOOLS_CMAKE_SCRIPTS_PATH "${CMAKE_CURRENT_LIST_FILE}" ABSOLUTE)
    get_filename_component(B3D_TOOLS_CMAKE_SCRIPTS_PATH "${B3D_TOOLS_CMAKE_SCRIPTS_PATH}" PATH)
    get_filename_component(B3D_TOOLS_PATH "${B3D_TOOLS_CMAKE_SCRIPTS_PATH}" PATH)
    get_filename_component(B3D_TOOLS_PATH "${B3D_TOOLS_PATH}" PATH)

    include("${B3D_TOOLS_PATH}/common/cmake/Qt.cmake")

    if(B3D_QT_FOUND AND NOT TARGET bombyx3d-tools-qt-common)
        add_subdirectory("${B3D_TOOLS_PATH}/common/qt" "${CMAKE_BINARY_DIR}/bombyx3d/tools/common/qt")
    endif()
endif()
