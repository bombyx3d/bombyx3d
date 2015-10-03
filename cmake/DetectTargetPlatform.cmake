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

set(B3D_WIN32 FALSE)
set(B3D_LINUX FALSE)
set(B3D_OSX FALSE)
set(B3D_EMSCRIPTEN FALSE)
set(B3D_IOS FALSE)
set(B3D_ANDROID FALSE)
set(B3D_TIZEN FALSE)
set(B3D_WINRT FALSE)
set(B3D_WINPHONE FALSE)
set(B3D_WINSTORE FALSE)

if("${CMAKE_VS_TARGET_PLATFORM}" MATCHES "WindowsPhone" OR "${CMAKE_SYSTEM_NAME}" MATCHES "WindowsPhone")
    set(B3D_WINRT TRUE)
    set(B3D_WINPHONE TRUE)
elseif("${CMAKE_VS_TARGET_PLATFORM}" MATCHES "WindowsStore" OR "${CMAKE_SYSTEM_NAME}" MATCHES "WindowsStore")
    set(B3D_WINRT TRUE)
    set(B3D_WINSTORE TRUE)
elseif(WIN32)
    set(B3D_WIN32 TRUE)
elseif(APPLE)
    set(B3D_OSX TRUE)
elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    set(B3D_LINUX TRUE)
else()
    message(FATAL_ERROR "Unsupported target platform.")
endif()


include(CheckSymbolExists)
if(MSVC)
    check_symbol_exists("_M_IX86" "" B3D_ARCH_X86)
    check_symbol_exists("_M_AMD64" "" B3D_ARCH_AMD64)
    check_symbol_exists("_M_ARM" "" B3D_ARCH_ARM)
else()
    check_symbol_exists("__i386__" "" B3D_ARCH_X86)
    check_symbol_exists("__x86_64__" "" B3D_ARCH_AMD64)
    check_symbol_exists("__arm__" "" B3D_ARCH_ARM)
endif()
