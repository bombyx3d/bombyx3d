#!/bin/sh

set -e

if [ "${Z_TARGET_PLATFORM}" = "qt5" ]; then
    sudo apt-add-repository -y ppa:beineri/opt-qt541
fi

if [ "${Z_DOXYGEN}" = "TRUE" ]; then
    # Add libreoffice PPA to get doxygen from
    # See http://stackoverflow.com/questions/27031958/doxygen-1-8-on-travis-ci
    sudo apt-add-repository -y ppa:libreoffice/libreoffice-4-2
else
    sudo apt-add-repository -y ppa:ubuntu-toolchain-r/test
fi

echo "Updating repositories..."
sudo apt-get -qq update
sudo apt-get -qq install libc6-i386

if [ "${Z_TARGET_PLATFORM}" = "qt5" ]; then
    sudo apt-get -qq install qt54base qt54multimedia qt54tools
fi

if [ "${Z_DOXYGEN}" = "TRUE" ]; then
    sudo apt-get -qq install --no-install-recommends doxygen graphviz
fi

if [ "${TRAVIS_OS_NAME}" = "linux" -a -( "${CC}" = "gcc" -o "${CXX}" = "g++" -) ]; then
    sudo apt-get -qq install g++-4.8
fi

wget http://www.cmake.org/files/v3.2/cmake-3.2.2-Linux-i386.tar.gz
tar -xzf cmake-3.2.2-Linux-i386.tar.gz
sudo cp -fR cmake-3.2.2-Linux-i386/* /usr
