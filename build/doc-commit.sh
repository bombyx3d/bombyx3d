#!/bin/sh

set -e

pwd=`pwd`
cd `dirname "$0"`
dir=`pwd`
cd "$pwd"

for file in `svn status "$dir/doc" | grep "^\!" | awk '{ print $2; }'`; do
    svn delete -q "$file"
done

for file in `svn status "$dir/doc" | grep "^\?" | awk '{ print $2; }'`; do
    svn add -q "$file"
done

svn commit --non-interactive --username zapolnov --password `cat ../id` \
    -q -m "[Travis] Update doxygen documentation." "$dir/doc"
