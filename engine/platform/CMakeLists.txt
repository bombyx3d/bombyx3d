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

if(B3D_WIN32)
    set(platform_name "win32")
    list(APPEND defines "B3D_PLATFORM_WIN32")
    list(APPEND libraries glfw)
    if(NOT B3D_USE_ANGLE)
        list(APPEND libraries glew)
    else()
        list(APPEND libraries angle)
        list(APPEND defines "B3D_USE_ANGLE")
    endif()
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

if(B3D_WINRT)
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/winrt")
    set(platform_name "winrt")
    list(APPEND defines "B3D_PLATFORM_WINRT")
    list(APPEND libraries angle)
    list(APPEND source_files
        shared/CxxThreadManager.cpp
        shared/CxxThreadManager.h
        shared/StdIoFile.cpp
        shared/StdIoFile.h
        shared/StdIoFileSystem.cpp
        shared/StdIoFileSystem.h
        winrt/App.xaml
        winrt/App.xaml.cpp
        winrt/App.xaml.h
        winrt/pch.cpp
        winrt/pch.h
    )
    if(B3D_WINPHONE)
        list(APPEND defines "B3D_PLATFORM_WINPHONE")
    endif()
    if(B3D_WINSTORE)
        list(APPEND defines "B3D_PLATFORM_WINSTORE")
    endif()
endif()

if(B3D_OSX)
    set(platform_name "osx")
    list(APPEND defines "B3D_PLATFORM_OSX")
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

if(B3D_LINUX)
    set(platform_name "linux")
    list(APPEND defines "B3D_PLATFORM_LINUX")
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

add_library(bombyx3d-platform STATIC EXCLUDE_FROM_ALL ${source_files})
b3d_set_source_groups(${source_files})

set_target_properties(bombyx3d-platform PROPERTIES
    CXX_STANDARD 11
    FOLDER "Bombyx3D"
    PROJECT_LABEL "bombyx3d-core-${platform_name}"
    OUTPUT_NAME "bombyx3d-core-${platform_name}"
)

target_link_libraries(bombyx3d-platform bombyx3d-core)
b3d_target_link_libraries(bombyx3d-platform glm ${libraries})
target_compile_definitions(bombyx3d-platform PUBLIC ${defines})
