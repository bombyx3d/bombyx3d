#!/bin/sh

set -e

# Add PPA with Qt 5.4
sudo apt-add-repository -y ppa:beineri/opt-qt541

# Install packages
sudo apt-get -qq update
sudo apt-get -qq install qt54base qt54multimedia qt54tools

# Install CMake
wget http://www.cmake.org/files/v3.2/cmake-3.2.2-Linux-i386.tar.gz
tar -xzf cmake-3.2.2-Linux-i386.tar.gz
sudo rm -f /usr/bin/cmake
sudo rm -f /usr/bin/ctest
sudo rm -f /usr/bin/cpack
sudo cp -fR cmake-3.2.2-Linux-i386/* /usr
sudo chmod a+x /usr/bin/cmake
sudo chmod a+x /usr/bin/ctest
sudo chmod a+x /usr/bin/cpack
