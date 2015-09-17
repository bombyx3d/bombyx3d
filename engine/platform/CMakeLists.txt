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

set(source_files)
set(defines)
set(libraries)

if(WIN32)
    list(APPEND defines "Z_PLATFORM_WIN32")
    list(APPEND libraries glfw glew)
    list(APPEND source_files
        shared/CxxThreadManager.cpp
        shared/CxxThreadManager.h
        shared/GlfwWrapper.cpp
        shared/GlfwWrapper.h
        shared/StdIoFile.cpp
        shared/StdIoFile.h
        shared/StdIoFileSystem.cpp
        shared/StdIoFileSystem.h
        win32/main.cpp
        win32/Win32GuiLogger.cpp
        win32/Win32GuiLogger.h
        win32/Win32ConsoleLogger.cpp
        win32/Win32ConsoleLogger.h
        win32/WinAPI.cpp
        win32/WinAPI.h
    )
endif()

if(APPLE)
    list(APPEND defines "Z_PLATFORM_OSX")
    list(APPEND libraries glfw)
    list(APPEND source_files
        shared/CxxThreadManager.cpp
        shared/CxxThreadManager.h
        shared/GlfwWrapper.cpp
        shared/GlfwWrapper.h
        shared/PosixLogger.cpp
        shared/PosixLogger.h
        shared/StdIoFile.cpp
        shared/StdIoFile.h
        shared/StdIoFileSystem.cpp
        shared/StdIoFileSystem.h
        osx/main.mm
    )
endif()

if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    list(APPEND defines "Z_PLATFORM_LINUX")
    list(APPEND libraries glfw glew)
    list(APPEND source_files
        shared/CxxThreadManager.cpp
        shared/CxxThreadManager.h
        shared/GlfwWrapper.cpp
        shared/GlfwWrapper.h
        shared/PosixLogger.cpp
        shared/PosixLogger.h
        shared/StdIoFile.cpp
        shared/StdIoFile.h
        shared/StdIoFileSystem.cpp
        shared/StdIoFileSystem.h
        linux/main.cpp
    )
endif()

add_library(platform STATIC EXCLUDE_FROM_ALL ${source_files})
z_set_source_groups(${source_files})

set_target_properties(platform PROPERTIES CXX_STANDARD 11)
target_link_libraries(platform engine glm ${libraries})
target_compile_definitions(platform PUBLIC ${defines})
