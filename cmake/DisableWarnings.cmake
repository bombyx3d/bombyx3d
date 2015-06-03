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

if(NOT __Z_DISABLE_WARNINGS_CMAKE_INCLUDED)
    set(__Z_DISABLE_WARNINGS_CMAKE_INCLUDED TRUE)

    include(CheckCCompilerFlag)
    include(CheckCXXCompilerFlag)

    macro(z_disable_warnings)

        if(NOT MSVC)
            check_c_compiler_flag("-w" HAVE_C_DISABLE_ALL_WARNINGS)
            if(HAVE_C_DISABLE_ALL_WARNINGS)
                set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
            endif()

            check_cxx_compiler_flag("-w" HAVE_CXX_DISABLE_ALL_WARNINGS)
            if(HAVE_CXX_DISABLE_ALL_WARNINGS)
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
            endif()
        else()
            add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
            add_definitions(-D_CRT_SECURE_NO_WARNINGS)
            add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
            add_definitions(-D_SCL_SECURE_NO_WARNINGS)

            check_c_compiler_flag("/W0" HAVE_MSVC_WARNING_LEVEL_0)
            if(HAVE_MSVC_WARNING_LEVEL_0)
                string(REGEX REPLACE "/W[0-4]" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
                string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
                set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W0")
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
            endif()

            check_c_compiler_flag("/w" HAVE_MSVC_DISABLE_ALL_WARNINGS)
            if(HAVE_MSVC_DISABLE_ALL_WARNINGS)
                string(REGEX REPLACE "/W[1-4]" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /w")
                string(REGEX REPLACE "/W[1-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w")
            endif()
        endif()

    endmacro()

endif()
