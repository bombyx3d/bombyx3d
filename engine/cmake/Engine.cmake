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

if(NOT __Z_ENGINE_CMAKE_INCLUDED)
    set(__Z_ENGINE_CMAKE_INCLUDED TRUE)

    get_filename_component(path "${CMAKE_CURRENT_LIST_FILE}" PATH)
    get_filename_component(path "${path}" ABSOLUTE)
    include("${path}/EnumOption.cmake")
    include("${path}/SetSourceGroups.cmake")
    include("${path}/Qt5.cmake")
    include("${path}/OpenGL.cmake")
    include("${path}/TargetPlatform.cmake")

    if(MSVC)
        add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
        add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    endif()

    if(Z_OPENGL_FOUND)
        add_definitions(-DZ_OPENGL_FOUND)
    endif()

    get_filename_component(engine_directory "${path}" PATH)
    include_directories("${engine_directory}")
    include_directories("${engine_directory}/3rdparty/glm")

    if(NOT __Z_ENGINE_DONT_ADD_SUBDIRECTORY AND NOT TARGET engine)
        add_subdirectory("${engine_directory}" engine)
    endif()

endif()
