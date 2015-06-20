#!/bin/sh

set -e

travis_dir=`pwd`

cleanup() {
    cd "$travis_dir"
}
trap cleanup EXIT INT QUIT HUP TERM

cd `dirname "$0"`
mkdir -p .cmake-build
cd .cmake-build

if [ "${TRAVIS_OS_NAME}" = "linux" ]; then
    if [ "${CC}" = "gcc" ]; then export CC=gcc-4.8; fi
    if [ "${CXX}" = "g++" ]; then export CXX=g++-4.8; fi
fi

echo CC=${CC}
echo CXX=${CXX}

/usr/bin/cmake \
    "-DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE" \
    "-DCMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH" \
    "-DZ_TARGET_PLATFORM=$Z_TARGET_PLATFORM" \
    "-DZ_BUILD_SAMPLES=YES" \
    ../..

make
