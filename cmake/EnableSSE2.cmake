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

include(CheckCXXCompilerFlag)
if(MSVC)
    if(CMAKE_SIZEOF_VOID_P GREATER 4)
        set(HAVE_COMPILER_SSE2_SUPPORT TRUE CACHE INTERNAL "" FORCE)
    else()
        check_cxx_compiler_flag("/arch:SSE2" HAVE_COMPILER_SSE2_SUPPORT)
        if(HAVE_COMPILER_SSE2_SUPPORT)
            add_definitions("/arch:SSE2")
        endif()
    endif()
else()
    check_cxx_compiler_flag("-msse2" HAVE_COMPILER_SSE2_SUPPORT)
    if(HAVE_COMPILER_SSE2_SUPPORT)
        add_definitions("-msse2")
    endif()
endif()
