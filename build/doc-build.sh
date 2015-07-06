#!/bin/sh
#
# Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com).
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

set -e

pwd=`pwd`
cd `dirname "$0"`
dir=`pwd`

cleanup() { cd "$pwd"; }
trap cleanup EXIT INT HUP QUIT TERM

## Checkout documentation

svn checkout -q https://github.com/bombyx3d/bombyx3d/branches/gh-pages/doxygen "$dir/doc"
find "$dir/doc/html" -mindepth 1 -name '.svn' -prune -o -print0 | xargs -0 rm -rf
find "$dir/doc/html" -mindepth 1 -name '.svn' -prune -o -type d -empty -print0 | xargs -0 rm -rf

## Update documentation

cd "$dir/.."
doxygen Doxyfile

## SVN: add new files, remove missing files

for file in `svn status "$dir/doc" | grep "^\!" | awk '{ print $2; }'`; do
    svn delete -q "$file"
done

for file in `svn status "$dir/doc" | grep "^\?" | awk '{ print $2; }'`; do
    svn add -q "$file"
done

## Commit documentation

svn commit --non-interactive --username zapolnov --password `cat "$dir/id"` \
    -q -m "[Travis] Update doxygen documentation." "$dir/doc"
