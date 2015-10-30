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

if(NOT _B3D_QT_INCLUDED)
    set(_B3D_QT_INCLUDED TRUE)

    set(B3D_QT_FOUND FALSE)
    if(POLICY CMP0063)
        cmake_policy(SET CMP0063 NEW)
    endif()

    find_package(Qt5Widgets)
    if(Qt5Widgets_FOUND)
        find_package(Qt5Xml REQUIRED)
        find_package(Qt5Network REQUIRED)
        find_package(Qt5Gui REQUIRED)
        find_package(Qt5Core REQUIRED)
        set(B3D_QT_FOUND TRUE)
        add_definitions(-DB3D_QT_FOUND)
    endif()

    set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
    set(CMAKE_AUTOMOC TRUE)

    macro(b3d_link_qt_libraries target)
        if(${B3D_QT_FOUND})
            target_link_libraries("${target}" Qt5::Xml Qt5::Network Qt5::Gui Qt5::Core Qt5::Widgets)
        endif()
    endmacro()
endif()
