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
project(QtSystem)

if(Z_TARGET_QT5)

    set(source_files
        io/QtFileReader.cpp
        io/QtFileReader.h
        io/QtFileSystem.cpp
        io/QtFileSystem.h
        utility/QtMacros.h
        QtSystem.cpp
        QtSystem.h
        common.cpp
    )

    z_set_source_groups(${source_files})
    add_library(libSystem STATIC EXCLUDE_FROM_ALL ${source_files})

    target_link_libraries(libSystem libEngineCore)
    set_target_properties(libSystem PROPERTIES OUTPUT_NAME "QtSystem" CXX_STANDARD 11)
    z_target_link_qt5(libSystem)

endif()
