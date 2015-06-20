#!/bin/sh

set -e

travis_dir=`pwd`

cleanup() {
    cd "$travis_dir"
}
trap cleanup EXIT INT QUIT HUP TERM

cd `dirname "$0"`

svn checkout -q https://github.com/zapolnov/game_engine/branches/gh-pages/doxygen doc
rm -rf doc/html

mkdir -p .cmake-build
cd .cmake-build

/usr/bin/cmake \
    "-DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE" \
    "-DCMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH" \
    "-DZ_TARGET_PLATFORM=$Z_TARGET_PLATFORM" \
    "-DZ_BUILD_SAMPLES=NO" \
    "-DZ_BUILD_DOCUMENTATION=YES" \
    ../..

make doc

for file in `svn status ../doc | grep "^\!" | awk '{ print $2; }'`; do
    svn delete -q "$file"
done

for file in `svn status ../doc | grep "^\?" | awk '{ print $2; }'`; do
    svn add -q "$file"
done

svn commit --non-interactive --username zapolnov --password `cat ../id` \
    -q -m "[Travis] Update doxygen documentation." ../doc
