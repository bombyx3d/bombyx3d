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

macro(b3d_copy_file target directory input)

    get_filename_component(filename "${input}" NAME)

    add_custom_command(TARGET
        "${target}"
    POST_BUILD
    COMMAND
        "${CMAKE_COMMAND}"
        -Dout="${directory}"
        -Din="${input}"
        -P "${B3D_CMAKE_SCRIPTS_PATH}/CopyIfNotExists.cmake"
    COMMENT
        "Copying ${filename}"
    )

endmacro()
