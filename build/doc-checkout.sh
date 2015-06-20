#!/bin/sh

set -e

pwd=`pwd`
cd `dirname "$0"`
dir=`pwd`
cd "$pwd"

svn checkout -q https://github.com/zapolnov/game_engine/branches/gh-pages/doxygen "$dir/doc"
find "$dir/doc/html" -mindepth 1 -name '.svn' -prune -o -print0 | xargs -0 rm -rf
find "$dir/doc/html" -mindepth 1 -name '.svn' -prune -o -type d -empty -print0 | xargs -0 rm -rf
