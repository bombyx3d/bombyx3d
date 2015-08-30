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
    'win32': [ 'auto', 'msvc2013', 'msvc2013-64', 'msvc2015', 'msvc2015-64', 'mingw' ]
}

def main():
    try:
        parser = argparse.ArgumentParser()
        parser.add_argument('-p', metavar='path', help='Path to the project directory')
        parser.add_argument('-c', metavar='compiler', help='Compiler to use to build the project')
        parser.add_argument('-t', metavar='target', help='Target to build ("debug" or "release")')
        parser.add_argument('-n', help="Generate project but don't build", action='store_true')
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

        target = args.t if args.t else 'release'
        if target != 'debug' and target != 'release':
            raise BuildError('Invalid target "%s". Valid values are: debug, release.')

        project = ProjectReader().read(projectPath)
        projectPath = os.path.abspath(projectPath)

        outputDirectory = os.path.join(projectPath, 'build')
        if platform != 'auto' and compiler != 'auto':
            outputDirectory = os.path.join(outputDirectory, '%s-%s-%s' % (platform, compiler, target))
        elif platform != 'auto':
            outputDirectory = os.path.join(outputDirectory, '%s-%s' % (platform, target))
        elif compiler != 'auto':
            outputDirectory = os.path.join(outputDirectory, '%s-%s' % (compiler, target))
        else:
            outputDirectory = os.path.join(outputDirectory, target)
        utility.createDirectory(outputDirectory)

        changed = cmake.generateCMakeLists(projectPath, outputDirectory, platform, compiler, target, project)

        if changed:
            cmake.runCMakeGenerate(projectPath, outputDirectory, platform, compiler, target, project)

        if not args.n:
            cmake.runCMakeBuild(projectPath, outputDirectory, platform, compiler, target, project)

    except BuildError as error:
        print('ERROR: %s' % error.message)
        sys.exit(1)

##############################################################################

if __name__ == '__main__':
    main()
