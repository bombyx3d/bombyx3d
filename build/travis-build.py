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
parser.add_argument('-p', '--platform', help='Value for Z_TARGET_PLATFORM')
parser.add_argument('-q', '--qt5path', help='Path to Qt5 installation')
parser.add_argument('-x', '--doxygen', help='Build documentation only', action='store_true')
args = parser.parse_args()

if args.build:
    build = args.build
else:
    build = 'Debug'

platform = False
if args.platform:
    platform = args.platform
elif args.doxygen:
    platform = 'dummy'

print('build    = %s' % build)

if platform:
    print('platform = %s' % platform)
if args.qt5path:
    print('qt5path  = %s' % args.qt5path)

#############################################################################################################
## Setup build environment

script_dir = os.path.dirname(os.path.abspath(__file__))
build_dir = os.path.join(script_dir, '.cmake-build')

try:
    os.makedirs(build_dir)
except:
    pass
os.chdir(build_dir)

if sys.platform == 'linux2':
    if os.environ['CC'] == 'gcc':
        os.environ['CC'] = 'gcc-4.8'
    if os.environ['CXX'] == 'g++':
        os.environ['CXX'] = 'g++-4.8'

if args.doxygen:
    subprocess.check_call('../doc-checkout.sh', shell=True)

#############################################################################################################
## Run CMake

cmake = [
    'cmake',
    '-DCMAKE_BUILD_TYPE=%s' % build,
    '-DZ_BUILD_SAMPLES=NO'
]

if platform:
    cmake.append('-DZ_TARGET_PLATFORM=%s' % platform)
if args.qt5path:
    cmake.append('-DCMAKE_PREFIX_PATH="%s"' % args.qt5path)

if args.doxygen:
    cmake.append('-DZ_BUILD_DOCUMENTATION=YES')
else:
    cmake.append('-DZ_BUILD_DOCUMENTATION=NO')

cmake.append('-DZ_BUILD_SAMPLES=YES')

if not on_windows:
    cmake.extend(['-G', '"Unix Makefiles"'])
else:
    cmake.extend(['-G', '"MinGW Makefiles"'])

cmake.append('../..')
subprocess.check_call(' '.join(cmake), shell=True)

#############################################################################################################
## Run make

make = 'make'
if on_windows:
    make = 'mingw32-make'

if args.doxygen:
    subprocess.check_call(('%s doc' % make), shell=True)
else:
    subprocess.check_call(make, shell=True)

#############################################################################################################
## Upload documentation

if args.doxygen:
    subprocess.check_call('../doc-commit.sh', shell=True)
