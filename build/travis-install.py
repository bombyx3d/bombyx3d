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
import sys

#############################################################################################################
## Parse command-line arguments

parser = argparse.ArgumentParser()
parser.add_argument('-c', '--cmake', help='Install CMake', action='store_true')
parser.add_argument('-q', '--qt5', help='Install Qt5', action='store_true')
parser.add_argument('-x', '--doxygen', help='Install Doxygen', action='store_true')
parser.add_argument('-g', '--gcc', help='Install GCC', action='store_true')
parser.add_argument('-l', '--clang', help='Install Clang', action='store_true')
parser.add_argument('-m', '--mingw', help='Install MinGW', action='store_true')
args = parser.parse_args()

if args.gcc and args.mingw:
    print('ERROR: --gcc and --mingw are mutually exclusive.')
    sys.exit(1)

#############################################################################################################
## Ubuntu Linux

if sys.platform == 'linux2':
    packages = ['libc6-i386']

    if args.qt5 and not args.mingw:
        subprocess.check_call('sudo apt-add-repository -y ppa:beineri/opt-qt541', shell=True)
        packages.extend(['qt54base','qt54multimedia','qt54tools'])

    if args.doxygen:
        # Add libreoffice PPA to get doxygen from
        # See http://stackoverflow.com/questions/27031958/doxygen-1-8-on-travis-ci
        subprocess.check_call('sudo apt-add-repository -y ppa:libreoffice/libreoffice-4-2', shell=True)
        packages.extend(['doxygen','graphviz'])

    if args.gcc:
        subprocess.check_call('sudo apt-add-repository -y ppa:ubuntu-toolchain-r/test', shell=True)
        packages.extend(['g++-4.8'])

    if args.clang:
        subprocess.check_call('sudo apt-add-repository -y ppa:h-rayflood/llvm', shell=True)
        packages.extend(['clang-3.4'])

    if args.mingw:
        subprocess.check_call('sudo apt-add-repository -y ppa:tobydox/mingw-x-precise', shell=True)
        packages.extend(['mingw-x-binutils','mingw-x-gcc'])
        if args.qt5:
            packages.extend(['mingw-x-qt'])

    print("Updating repositories...")
    subprocess.check_call('sudo apt-get -qq update', shell=True)

    subprocess.check_call(('sudo apt-get -qq install --no-install-recommends %s' % (' '.join(packages))), shell=True)

    if args.cmake:
        subprocess.check_call('wget http://www.cmake.org/files/v3.2/cmake-3.2.2-Linux-i386.tar.gz', shell=True)
        subprocess.check_call('tar -xzf cmake-3.2.2-Linux-i386.tar.gz', shell=True)
        subprocess.check_call('sudo cp -fR cmake-3.2.2-Linux-i386/* /usr', shell=True)

#############################################################################################################
## Apple OSX

if sys.platform == 'darwin':
    subprocess.check_call('brew update', shell=True)

    if args.cmake:
        subprocess.call('brew unlink cmake', shell=True)
        subprocess.check_call('brew install cmake', shell=True)

    if args.gcc:
        print('ERROR: --gcc is not supported on OSX host.')
    if args.mingw:
        print('ERROR: --mingw is not supported on OSX host.')

    if args.qt5:
        subprocess.check_call('brew install qt5', shell=True)
        subprocess.check_call('chmod -R 755 /usr/local/opt/qt5/*', shell=True)

    if args.doxygen:
        subprocess.check_call('brew install doxygen graphviz', shell=True)
