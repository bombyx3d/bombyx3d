#!/usr/bin/env python
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

import errno
import subprocess
import os
import sys

script_path = os.path.dirname(os.path.abspath(__file__))
project_path = os.path.dirname(script_path)
build_path = os.path.join(project_path, 'cmake-build', 'winphone81')

try:
    os.makedirs(build_path)
except OSError as e:
    if e.errno == errno.EEXIST and os.path.isdir(build_path):
        pass
    else:
        raise

os.chdir(build_path)

command = [
    'cmake',
    '-G', 'Visual Studio 12 2013 ARM',
    '-DCMAKE_SYSTEM_NAME=WindowsPhone',
    '-DCMAKE_SYSTEM_VERSION=8.1',
    project_path
]
print(' '.join(command))
subprocess.check_call(command)

command = [
    'MSBuild',
    os.path.join(build_path, 'Bombyx3D.sln'),
    '/t:sample'
]
print(' '.join(command))
#subprocess.check_call(command)
