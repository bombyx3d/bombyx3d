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

/usr/bin/cmake \
    "-DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE" \
    "-DCMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH" \
    "-DZ_TARGET_PLATFORM=$Z_TARGET_PLATFORM" \
    "-DZ_BUILD_SAMPLES=YES" \
    ../..

make
