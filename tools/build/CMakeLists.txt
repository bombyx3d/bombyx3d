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

cmake_minimum_required(VERSION 3.2)
project("${projectName}")

include(CheckTypeSize)
include(CheckIncludeFile)

find_package(Threads REQUIRED)
find_package(OpenGL)
find_package(OpenAL)


##############################################################################
# Re-run the build.py script to update CMakeLists.txt if required

add_custom_target("${projectName}_prebuild" ALL
    "${pythonExecutable}" "${buildPy}"
        -n
        -p "${projectPath}"
        -c "${compiler}"
        -t "${target}"
        "${platform}"
    WORKING_DIRECTORY
        "${projectPath}"
    COMMENT
        "Running build.py"
)


##############################################################################
# Set global compiler flags

if(MSVC)
    # CMake by default adds /W3 to CMAKE_C_FLAGS and CMAKE_CXX_FLAGS variables
    # when compiling with MSVC. This results in a compiler warning when we
    # add /W0 or /W4 to compiler command line. So we have to remove default
    # setting from these variables.
    string(REGEX REPLACE "/W[0-4]" "" CMAKE_C_FLAGS "$${CMAKE_C_FLAGS}")
    string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "$${CMAKE_CXX_FLAGS}")

    # Microsoft has introduced "secure" versions of some C standard library
    # functions, like strcpy(). Their compiler reports a warning when it finds
    # usages of "insecure" versions. As these "secure" functions are not
    # available on other platforms, they can't be used in portable code.
    # These defines tell the compiler to shut up.
    add_definitions(
        -D_CRT_SECURE_NO_WARNINGS=1
        -D_CRT_SECURE_NO_DEPRECATE=1
        -D_CRT_NONSTDC_NO_DEPRECATE=1
    )

    # We use /W4 flag to enable most MSVC compiler warnings, but some
    # warnings included in this level are useless and annoying. So we have to
    # explicitly disable these particular warnings.
    add_definitions(
        /wd4510    # default constructor could not be generated
        /wd4512    # assignment operator could not be generated
        /wd4610    # object can never be instantiated
    )
endif()

# zlib uses this flag
check_include_file(unistd.h HAVE_UNISTD_H)
if(HAVE_UNISTD_H)
    add_definitions("-DZ_HAVE_UNISTD_H")
endif()

set(CMAKE_REQUIRED_DEFINITIONS "-D_LARGEFILE64_SOURCE=1")
check_type_size(off64_t OFF64_T)
if(HAVE_OFF64_T)
    add_definitions("-D_LARGEFILE64_SOURCE=1")
else()
    set(CMAKE_REQUIRED_DEFINITIONS)
endif()


##############################################################################
# Specify defines from the project file

add_definitions(
${defines})


##############################################################################
# Create target for third-party libraries

add_library("${projectName}_libraries" STATIC
${librarySourceFiles})

set_target_properties("${projectName}_libraries" PROPERTIES CXX_STANDARD 11)
add_dependencies("${projectName}_libraries" "${projectName}_prebuild")

# Disable all warnings for third-party code
if(MSVC)
    target_compile_options("${projectName}_libraries" PUBLIC "/w" "/W0")
else()
    target_compile_options("${projectName}_libraries" PUBLIC "-w")
endif()


##############################################################################
# Create target for the project

add_executable("${projectName}" MACOSX_BUNDLE
${projectSourceFiles})

set_target_properties("${projectName}" PROPERTIES CXX_STANDARD 11)
add_dependencies("${projectName}" "${projectName}_prebuild")
target_link_libraries("${projectName}" "${projectName}_libraries")

# Enable all warnings for project code
if(MSVC)
    target_compile_options("${projectName}" PUBLIC "/W4")
else()
    target_compile_options("${projectName}" PUBLIC "-pedantic" "-Wall" "-Wextra" "-Wconversion")
    if(NOT CMAKE_COMPILER_IS_GNUCC AND NOT CMAKE_COMPILER_IS_GNUCXX)
        # Implementation of the -Wshadow flag in GCC is too strict.
        # On the other side, this flag is quite usable with Clang.
        target_compile_options("${projectName}" PUBLIC "-Wshadow")
    endif()
endif()


##############################################################################
# Specify header search paths

target_include_directories("${projectName}_libraries" SYSTEM BEFORE PUBLIC
${libraryIncludeDirectories})

target_include_directories("${projectName}_libraries" BEFORE PUBLIC
${projectIncludeDirectories})

if(OPENGL_FOUND)
    target_include_directories("${projectName}_libraries" SYSTEM PUBLIC $${OPENGL_INCLUDE_DIR})
endif()

if(OPENAL_FOUND)
    target_include_directories("${projectName}_libraries" SYSTEM PUBLIC $${OPENAL_INCLUDE_DIR})
endif()


##############################################################################
# Link with system libraries

target_link_libraries("${projectName}" $${CMAKE_THREAD_LIBS_INIT})
target_link_libraries("${projectName}_libraries" $${CMAKE_THREAD_LIBS_INIT})

target_link_libraries("${projectName}" $${CMAKE_DL_LIBS})
target_link_libraries("${projectName}_libraries" $${CMAKE_DL_LIBS})

find_library(LIBM m)
if(LIBM)
    target_link_libraries("${projectName}" $${LIBM})
    target_link_libraries("${projectName}_libraries" $${LIBM})
endif()

if(OPENGL_FOUND)
    target_link_libraries("${projectName}" $${OPENGL_INCLUDE_DIR})
    target_link_libraries("${projectName}_libraries" $${OPENGL_INCLUDE_DIR})
endif()

if(OPENAL_FOUND)
    target_link_libraries("${projectName}" $${OPENAL_INCLUDE_DIR})
    target_link_libraries("${projectName}_libraries" $${OPENAL_INCLUDE_DIR})
endif()
