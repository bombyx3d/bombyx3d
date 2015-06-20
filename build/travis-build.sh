#!/bin/sh

set -e

travis_dir=`pwd`

cleanup() {
    cd "$travis_dir"
}
trap cleanup EXIT INT QUIT HUP TERM

cd `dirname "$0"`

mkdir -p ".cmake-$Z_TARGET_PLATFORM-$CMAKE_BUILD_TYPE"
cd ".cmake-$Z_TARGET_PLATFORM-$CMAKE_BUILD_TYPE"

cmake \
    "-DCMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH" \
    "-DZ_TARGET_PLATFORM=$Z_TARGET_PLATFORM" \
    "-DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE" \
    ../..

make
