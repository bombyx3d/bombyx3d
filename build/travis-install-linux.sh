#!/bin/sh

set -e

# Install CMake
wget http://www.cmake.org/files/v3.2/cmake-3.2.2-Linux-i386.tar.gz
tar -xzf cmake-3.2.2-Linux-i386.tar.gz
sudo cp -fR cmake-3.2.2-Linux-i386/* /usr

ldd /usr/bin/cmake

# Add PPA with Qt 5.4
sudo apt-add-repository -y ppa:beineri/opt-qt541

# Install packages
sudo apt-get -qq update
sudo apt-get -qq install qt54base qt54multimedia qt54tools
