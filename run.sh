#!/bin/sh
set -e
mkdir -p cmake-build
cd cmake-build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j 4
cd ../samples/sample/data
../../../cmake-build/samples/sample/sample
