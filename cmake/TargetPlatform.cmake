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

if(NOT __Z_TARGET_PLATFORM_CMAKE_INCLUDED)
    set(__Z_TARGET_PLATFORM_CMAKE_INCLUDED TRUE)

    get_filename_component(path "${CMAKE_CURRENT_LIST_FILE}" PATH)
    include("${path}/Qt5.cmake")
    include("${path}/EnumOption.cmake")

    set(Z_SUPPORTED_PLATFORMS "dummy")
    set(Z_DEFAULT_PLATFORM "dummy")

    #######################
    # Supported platforms

    if(${Z_QT5_FOUND})
        list(APPEND Z_SUPPORTED_PLATFORMS "qt5")
    endif()

    #######################
    # Default platform

    if (${Z_QT5_FOUND})
        set(Z_DEFAULT_PLATFORM "qt5")
    endif()

    #######################

    z_enum_option(Z_TARGET_PLATFORM "Target platform" "${Z_DEFAULT_PLATFORM}" "${Z_SUPPORTED_PLATFORMS}")

    set(Z_TARGET_DUMMY FALSE)
    set(Z_TARGET_QT5 FALSE)

    if(Z_TARGET_PLATFORM STREQUAL "dummy")
        set(Z_TARGET_DUMMY TRUE)
        add_definitions(-DZ_TARGET_DUMMY)
    elseif(Z_TARGET_PLATFORM STREQUAL "qt5")
        set(Z_TARGET_QT5 TRUE)
        add_definitions(-DZ_TARGET_QT5)
    endif()

endif()
