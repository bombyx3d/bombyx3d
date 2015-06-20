#!/bin/sh

set -e

brew update
brew unlink cmake
brew install cmake
brew install qt5
chmod -R 755 /usr/local/opt/qt5/*
