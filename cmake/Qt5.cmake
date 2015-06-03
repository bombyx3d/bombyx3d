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

if(NOT __Z_FIND_QT5_CMAKE_INCLUDED)
    set(__Z_FIND_QT5_CMAKE_INCLUDED TRUE)

    set(Z_QT5_FOUND NO)

    find_package(Qt5Widgets)
    if(Qt5Widgets_FOUND)
        find_package(Qt5Core)
        find_package(Qt5Gui)
        find_package(Qt5Network)
        find_package(Qt5Multimedia)
        find_package(Qt5OpenGL)
        if(Qt5Core_FOUND AND Qt5Gui_FOUND AND Qt5Network_FOUND AND Qt5Multimedia_FOUND AND Qt5OpenGL_FOUND)
            set(Z_QT5_FOUND YES)
        endif()
    endif()

    macro(z_target_link_qt5 target)
        if(Z_QT5_FOUND)
            target_link_libraries("${target}" Qt5::Core Qt5::Gui Qt5::Network Qt5::Multimedia Qt5::OpenGL)
            if(MSVC)
                target_compile_options("${target}" PRIVATE /wd4458)
            endif()
        endif()
    endmacro()

    macro(z_qt5_sources)
        if(NOT Z_TARGET_QT5)
            set_source_files_properties(${ARGN} PROPERTIES HEADER_FILE_ONLY TRUE)
        endif()
    endmacro()

endif()
