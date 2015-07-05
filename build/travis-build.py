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
parser.add_argument('-b', '--build', help='Value for CMAKE_BUILD_TYPE')
parser.add_argument('-s', '--system', help='Value for "system" enumeration')
parser.add_argument('-q', '--qt5path', help='Path to Qt5 installation')
parser.add_argument('-x', '--doxygen', help='Build documentation only', action='store_true')
args = parser.parse_args()

#############################################################################################################
## Change to the script directory

dir = os.path.dirname(os.path.realpath(__file__))
os.chdir(dir)

#############################################################################################################
## Build documentation if requested

if args.doxygen:
    subprocess.check_call('./doc-build.sh', shell=True)
    os.exit(0)

#############################################################################################################
## Generate and build the project

if sys.platform == 'linux2':
    if os.environ['CC'] == 'gcc':
        os.environ['CC'] = 'gcc-4.8'
    if os.environ['CXX'] == 'g++':
        os.environ['CXX'] = 'g++-4.8'

generator = 'CMake 3.2+'

if on_windows:
    cmake_build_tool = 'MinGW (32-bit)'
else:
    cmake_build_tool = 'Unix Makefiles'

print('generator        = %s' % generator)
if cmake_build_tool:
    print('cmake_build_tool = %s' % cmake_build_tool)
if args.build:
    print('build            = %s' % args.build)
if args.system:
    print('system           = %s' % args.system)
if args.qt5path:
    print('qt5path          = %s' % args.qt5path)

command = 'java -jar zbt/bin/zbt.jar'
command += ' -v'                                    # Verbose error reporting
command += ' -b'                                    # Batch mode
command += ' -p ../samples/1.match3'                # Path to the project
command += (' -g "%s"' % generator)                 # Generator to use
command += ' --build'

if cmake_build_tool:
    command += (' --cmake-build-tool "%s"' % cmake_build_tool)
if args.build:
    command += (' --cmake-build-type "%s"' % args.build)
if args.qt5path:
    command += (' --cmake-qt5-path "%s"' % args.qt5path)

print(command)
subprocess.check_call(command, shell=True)
