#!/usr/bin/env python
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

import argparse
import subprocess
import os
import sys

#############################################################################################################
## Determine host platform

on_linux = sys.platform == 'linux2'
on_osx = sys.platform == 'darwin'
on_windows = (sys.platform == 'win32' or sys.platform == 'cygwin')

#############################################################################################################
## Parse command-line arguments

parser = argparse.ArgumentParser()
parser.add_argument('-b', '--cmake_build_type', help='Value for CMAKE_BUILD_TYPE')
parser.add_argument('-g', '--cmake_generator', help='CMake generator to use')
parser.add_argument('-l', '--clang', help='Compile with Clang')
parser.add_argument('-q', '--qt5path', help='Path to Qt5 installation')
parser.add_argument('-x', '--doxygen', help='Build documentation only', action='store_true')
args = parser.parse_args()

#############################################################################################################
## Change to the project directory

dir = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
os.chdir(dir)

#############################################################################################################
## Generate the project

generator = 'com.zapolnov.buildsystem.build.qt5.Qt5CMakeGenerator'
project_dir = 'qt5-cmake'

command = 'java -jar buildsystem.jar'
command += ' -b'                                    # Batch mode
command += ' -p .'                                  # Path to the project
command += (' -g "%s"' % generator)                 # Generator to use
print(command)
subprocess.check_call(command, shell=True)

#############################################################################################################
## Build documentation if requested

if args.doxygen:
    if on_windows:
        subprocess.check_call('build\\doc-build.cmd', shell=True)
    else:
        subprocess.check_call('build/doc-build.sh', shell=True)
    sys.exit(0)

#############################################################################################################
## Build the project

os.mkdir('build/%s-build' % project_dir)
os.chdir('build/%s-build' % project_dir)

if sys.platform == 'linux2':
    if args.clang:
        os.environ['CC'] = 'clang-3.4'
        os.environ['CXX'] = 'clang++-3.4'
    else:
        os.environ['CC'] = 'gcc-4.8'
        os.environ['CXX'] = 'g++-4.8'

command = 'cmake'
if args.cmake_generator:
    command += (' -G "%s"' % args.cmake_generator)
if args.cmake_build_type:
    command += (' -DCMAKE_BUILD_TYPE="%s"' % args.cmake_build_type)
if args.qt5path:
    command += (' -DCMAKE_PREFIX_PATH="%s"' % args.qt5path)
command += (' ../%s' % project_dir)
print(command)
subprocess.check_call(command, shell=True)

command = ('cmake --build .')
print(command)
subprocess.check_call(command, shell=True)
