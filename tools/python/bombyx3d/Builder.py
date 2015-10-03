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
import glob
import subprocess
import argparse
import os
import sys
from StripUtf8Bom import stripUtf8BomFromDirectory

def makePath(path):
    try:
        os.makedirs(path)
    except OSError as e:
        if e.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise

def printCommand(command):
    args = []
    for arg in command:
        if not ' ' in arg and not '\\' in arg and not '"' in arg:
            args.append(arg)
        else:
            args.append('"%s"' % arg.replace('\\', '\\\\').replace('"', '\\"'))
    print(' '.join(args))

def runCommand(command):
    printCommand(command)
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True)
    commandOutput = ''
    while True:
        outputLine = process.stdout.readline()
        if outputLine == b'' and process.poll() != None:
            break
        commandOutput = commandOutput + outputLine
        sys.stdout.write(outputLine)
    process.communicate()
    if process.returncode != 0:
        print('ERROR: command \"%s\" has exited with code %d' % (command[0], process.returncode))
        sys.exit(1)
    return commandOutput

class Builder:
    def __init__(self):
        self.projectPath = None
        self.outputPath = None
        self.cmakeBuildType = 'Release'
        self.stripUtf8Bom = False

        if sys.platform == 'linux2':
            self.targetPlatform = 'linux32'
            self.targetPlatformChoices = [ 'linux32', 'linux64', 'emscripten', 'android', 'pnacl' ]
        elif sys.platform == 'darwin':
            self.targetPlatform = 'osx'
            self.targetPlatformChoices = [ 'osx', 'emscripten', 'android', 'pnacl' ]
        elif sys.platform == 'win32':
            self.targetPlatform = 'win32'
            self.targetPlatformChoices = [ 'win32', 'win32-msvc', 'win64', 'win64-msvc',
                'emscripten', 'android', 'winphone81', 'winstore81', 'winstore81-x64', 'pnacl' ]
        else:
            print('FATAL ERROR: unsupported host platform. Please run this script on Windows, Linux or OSX.')
            sys.exit(1)

    def parseArguments(self):
        parser = argparse.ArgumentParser()
        if not self.projectPath:
            parser.add_argument('-p', '--project', help='Path to the source directory of the project')
        if not self.outputPath:
            parser.add_argument('-o', '--output', help='Path to the build directory')
        parser.add_argument('-c', '--configuration', help='Value for CMAKE_BUILD_TYPE',
            choices=['Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel'], default=self.cmakeBuildType)
        parser.add_argument('-t', '--target', help='Target platform',
            choices=self.targetPlatformChoices, default=self.targetPlatform)
        parser.add_argument('-x', '--strip-utf8-bom', help='Strip UTF-8 byte order mark from source files',
            action='store_true')
        args = parser.parse_args()
        self.cmakeBuildType = args.configuration
        self.targetPlatform = args.target
        if not self.projectPath and args.project:
            self.projectPath = os.path.abspath(args.project)
        if not self.outputPath and args.output:
            self.outputPath = os.path.abspath(args.output)
        if args.strip_utf8_bom:
            self.stripUtf8Bom = True

    def build(self):
        if self.projectPath:
            sourcePath = self.projectPath
        else:
            sourcePath = os.path.abspath(os.getcwd());

        if self.outputPath:
            buildPath = self.outputPath
        else:
            buildPath = os.path.join(os.path.abspath(os.getcwd()), 'cmake-build',
                self.targetPlatform, self.cmakeBuildType)

        installPath = os.path.join(buildPath, 'DISTRIBUTION_PACKAGE')

        print('-------------------------------------------------------------------------------')
        print('Project directory: %s' % sourcePath)
        print('Output directory:  %s' % buildPath)
        print('Target:            %s' % self.targetPlatform)
        print('Configuration:     %s' % self.cmakeBuildType)
        print('-------------------------------------------------------------------------------')

        if self.stripUtf8Bom:
            print('Stripping UTF-8 BOM from source files')
            stripUtf8BomFromDirectory(sourcePath)
            print('Done stripping UTF-8 BOM')

        print('mkdir -p "%s"' % buildPath)
        makePath(buildPath)

        print('cd "%s"' % buildPath)
        os.chdir(buildPath)

        makefilesGenerator = 'MinGW Makefiles' if sys.platform == 'win32' else 'Unix Makefiles'
        EXTRA_CMAKE_OPTIONS = {
            'win32': [ '-G', makefilesGenerator, '-DB3D_GCC_BITS=32' ],
            'win32-msvc': [ '-G', 'Visual Studio 12 2013' ],
            'win64': [ '-G', makefilesGenerator, '-DB3D_GCC_BITS=64' ],
            'win64-msvc': [ '-G', 'Visual Studio 12 2013 Win64' ],
            'linux32': [ '-G', makefilesGenerator, '-DB3D_GCC_BITS=32' ],
            'linux64': [ '-G', makefilesGenerator, '-DB3D_GCC_BITS=64' ],
            'emscripten': [ '-G', makefilesGenerator ],
            'pnacl': [ '-G', makefilesGenerator ],
            'osx': [ '-G', 'Xcode' ],
            'ios': [ '-G', 'Xcode' ],
            'android': [ '-G', makefilesGenerator ],
            'winphone81': [ 'G', 'Visual Studio 12 2013 ARM',
                '-DCMAKE_SYSTEM_NAME=WindowsPhone', '-DCMAKE_SYSTEM_VERSION=8.1' ],
            'winstore81': [ 'G', 'Visual Studio 12 2013',
                '-DCMAKE_SYSTEM_NAME=WindowsStore', '-DCMAKE_SYSTEM_VERSION=8.1' ],
            'winstore81-x64': [ 'G', 'Visual Studio 12 2013 Win64',
                '-DCMAKE_SYSTEM_NAME=WindowsStore', '-DCMAKE_SYSTEM_VERSION=8.1' ],
        }
        command = [ 'cmake' ]
        command.extend(EXTRA_CMAKE_OPTIONS[self.targetPlatform])
        command.extend([
            '-DCMAKE_BUILD_TYPE=%s' % self.cmakeBuildType,
            '-DCMAKE_INSTALL_PREFIX=%s' % installPath,
            sourcePath
        ])
        runCommand(command)

        makefilesBuild = [ 'cmake', '--build', buildPath, '--', '-j', '4' ]
        xcodeBuild = [ 'cmake', '--build', buildPath ]
        msvcBuild = [ 'MSBuild', next(iter(glob.glob(os.path.join(buildPath, '*.sln'))), None), '/nologo',
                '/verbosity:minimal', '/maxcpucount', ('/p:Configuration=%s' % self.cmakeBuildType) ]

        BUILD_COMMAND = {
            'win32': makefilesBuild,
            'win32-msvc': msvcBuild,
            'win64': makefilesBuild,
            'win64-msvc': msvcBuild,
            'linux32': makefilesBuild,
            'linux64': makefilesBuild,
            'emscripten': makefilesBuild,
            'pnacl': makefilesBuild,
            'osx': xcodeBuild,
            'ios': xcodeBuild,
            'android': makefilesBuild,
            'winphone81': msvcBuild,
            'winstore81': msvcBuild,
            'winstore81-x64': msvcBuild,
        }
        runCommand(BUILD_COMMAND[self.targetPlatform])
