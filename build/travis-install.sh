#!/bin/sh

set -e

if [ "$TRAVIS_OS_NAME" == "linux" ]; then
    apt-cache search qt5 | grep dev

    sudo apt-get update -qq
    sudo apt-get install -qq -y libqt5-dev;
fi
