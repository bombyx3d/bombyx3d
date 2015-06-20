#!/usr/bin/env python

import argparse
import platform
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

print('build    = %s' % build)

if args.platform:
    print('platform = %s' % args.platform)
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
        os.environ['CXX'] = 'gcc-4.8'

if args.doxygen:
    subprocess.check_call('../doc-checkout.sh', shell=True)

#############################################################################################################
## Run CMake

cmake = [
    'cmake',
    '-DCMAKE_BUILD_TYPE=%s' % build,
    '-DZ_BUILD_SAMPLES=NO'
]

if args.platform:
    cmake.append('-DZ_TARGET_PLATFORM=%s' % args.platform)
if args.qt5path:
    cmake.append('-DCMAKE_PREFIX_PATH=%s' % args.qt5path)

if args.doxygen:
    cmake.append('-DZ_BUILD_DOCUMENTATION=YES')
else:
    cmake.append('-DZ_BUILD_DOCUMENTATION=NO')

cmake.append('-DZ_BUILD_SAMPLES=YES')

if not on_windows:
    cmake.extend(['-G', 'Unix Makefiles'])
else:
    cmake.extend(['-G', 'MinGW Makefiles'])

cmake.append('../..')
subprocess.check_call(cmake, shell=True)

#############################################################################################################
## Run make

make = 'make'
if on_windows:
    make = 'mingw32-make'

if args.doxygen:
    subprocess.check_call([make, 'doc'], shell=True)
else:
    subprocess.check_call([make], shell=True)

#############################################################################################################
## Upload documentation

subprocess.check_call('../doc-commit.sh', shell=True)
