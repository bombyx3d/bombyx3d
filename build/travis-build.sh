#!/bin/sh

set -e

cd `dirname "$0"`

mkdir -p ".cmake-$Z_TARGET_PLATFORM-$CMAKE_BUILD_TYPE"
cd ".cmake-$Z_TARGET_PLATFORM-$CMAKE_BUILD_TYPE"

cmake \
    "-DZ_TARGET_PLATFORM=$Z_TARGET_PLATFORM" \
    "-DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE" \
    ../..

make
