#!/usr/bin/env python

import argparse
import subprocess
import sys

#############################################################################################################
## Parse command-line arguments

parser = argparse.ArgumentParser()
parser.add_argument('-q', '--qt5', help='Install Qt5', action='store_true')
parser.add_argument('-x', '--doxygen', help='Install Doxygen', action='store_true')
parser.add_argument('-g', '--gcc', help='Install GCC', action='store_true')
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

    if args.mingw:
        subprocess.check_call('sudo apt-add-repository -y ppa:tobydox/mingw-x-precise', shell=True)
        packages.extend(['mingw-x-binutils','mingw-x-gcc'])
        if args.qt5:
            packages.extend(['mingw-x-qt'])

    print("Updating repositories...")
    subprocess.check_call('sudo apt-get -qq update', shell=True)

    subprocess.check_call(('sudo apt-get -qq install --no-install-recommends %s' % (' '.join(packages))), shell=True)

    subprocess.check_call('wget http://www.cmake.org/files/v3.2/cmake-3.2.2-Linux-i386.tar.gz', shell=True)
    subprocess.check_call('tar -xzf cmake-3.2.2-Linux-i386.tar.gz', shell=True)
    subprocess.check_call('sudo cp -fR cmake-3.2.2-Linux-i386/* /usr', shell=True)

#############################################################################################################
## Apple OSX

if sys.platform == 'darwin':
    subprocess.check_call('brew update', shell=True)
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
