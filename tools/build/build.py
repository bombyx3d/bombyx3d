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

import sys
import os
import argparse
import utility

toolsDir = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
sys.path.insert(0, os.path.join(toolsDir, 'libs', 'pyyaml', 'lib'))

from project import Project, ProjectReader
from utility import BuildError

import cmake

##############################################################################

compilers = {
    'auto': [ 'auto' ],
    'win32': [ 'auto', 'msvc2013' ]
}

def main():
    try:
        parser = argparse.ArgumentParser()
        parser.add_argument('-p', metavar='path', help='Path to the project directory')
        parser.add_argument('-c', metavar='compiler', help='Compiler to use to build the project')
        parser.add_argument('platform', nargs='?', metavar='platform', help='Target platform')
        args = parser.parse_args()

        projectPath = '.'
        if args.p:
            projectPath = args.p
            if not os.path.exists(projectPath):
                raise BuildError('Directory does not exist: "%s".' % projectPath)
            elif not os.path.isdir(projectPath):
                raise BuildError('Not a directory: "%s".' % projectPath)

        platform = args.platform if args.platform else 'auto'
        if not platform in compilers:
            raise BuildError('Unknown platform "%s". Valid values are: %s'
                % (platform, ', '.join(sorted(compilers.keys()))))

        compiler = args.c if args.c else 'auto'
        if not compiler in compilers[platform]:
            raise BuildError('Compiler "%s" is not supported for platform "%s". Valid values are: %s'
                % (compiler, platform, ', '.join(sorted(compilers[platform]))))

        project = ProjectReader().read(projectPath)

        projectPath = os.path.abspath(projectPath)
        outputDirectory = os.path.join(projectPath, '.build')
        utility.createDirectory(outputDirectory)

        cmake.generateCMakeLists(projectPath, outputDirectory, platform, compiler, project)

    except BuildError as error:
        print('ERROR: %s' % error.message)
        sys.exit(1)

##############################################################################

if __name__ == '__main__':
    main()
