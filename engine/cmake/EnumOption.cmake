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

if(NOT __Z_ENUM_OPTION_CMAKE_INCLUDED)
    set(__Z_ENUM_OPTION_CMAKE_INCLUDED TRUE)

    macro(z_enum_option name description value values)
        set("${name}" "${value}" CACHE STRING "${description} (possible values are: ${values})")

        set(valid NO)
        foreach(possible_value ${values})
            if("${${name}}" STREQUAL "${possible_value}")
                set(valid YES)
                break()
            endif()
        endforeach()

        if(NOT ${valid})
            message(FATAL_ERROR "Invalid value \"${${name}}\" for option \"${name}\". Valid values are: ${values}")
        endif()
    endmacro()

endif()
