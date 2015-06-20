#!/bin/sh

set -e

sudo apt-add-repository -y ppa:beineri/opt-qt541
sudo apt-add-repository -y ppa:ubuntu-toolchain-r/test

sudo apt-get -qq update
sudo apt-get -qq install g++-4.8 libc6-i386 qt54base qt54multimedia qt54tools

if [ "$CC" = "gcc" ]; then export CC=gcc-4.8; fi
if [ "$CXX" = "g++" ]; then export CXX=g++-4.8; fi

wget http://www.cmake.org/files/v3.2/cmake-3.2.2-Linux-i386.tar.gz
tar -xzf cmake-3.2.2-Linux-i386.tar.gz
sudo cp -fR cmake-3.2.2-Linux-i386/* /usr
