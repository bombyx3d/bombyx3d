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
project(Sample1)

set(source_files
    Game.cpp
    Game.h
    Match3Field.cpp
    Match3Field.h
    Match3Listener.h
    Match3SpriteFactory.h
    Match3View.cpp
    Match3View.h
)

z_set_source_groups(${source_files})
add_executable(Sample1 ${source_files})

set_target_properties(Sample1 PROPERTIES CXX_STANDARD 11)
target_link_libraries(Sample1 libengine libMain)

get_filename_component(path "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
if(WIN32)
    string(REPLACE "\\" "/" path "${path}")
endif()
target_compile_definitions(Sample1 PUBLIC "Z_DEBUG_ASSETS_PATH=\"${path}\"")
