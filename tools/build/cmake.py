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
import subprocess
import string
import textwrap
import utility

def escape(string):
    return string.replace('\\', '\\\\').replace('"', '\\"')

def escapePath(string):
    if not sys.platform == 'win32':
        return escape(string)
    else:
        return escape(string.replace('\\', '/'))

def generateCMakeLists(projectPath, outputDirectory, platform, compiler, project):
    scriptDir = os.path.dirname(os.path.realpath(__file__))

    projectSourceFiles = ''
    for file in project.projectSources:
        projectSourceFiles += '    "%s"\n' % escapePath(os.path.realpath(file))

    librarySourceFiles = ''
    for file in project.librarySources:
        librarySourceFiles += '    "%s"\n' % escapePath(os.path.realpath(file))

    projectIncludeDirectories = ''
    for path in project.projectIncludeDirectories:
        projectIncludeDirectories += '    "%s"\n' % escapePath(os.path.realpath(path))

    libraryIncludeDirectories = ''
    for path in project.libraryIncludeDirectories:
        libraryIncludeDirectories += '    "%s"\n' % escapePath(os.path.realpath(path))

    defines = ''
    for define in project.defines:
        defines += '    "-D%s"\n', escape(define)

    variables = {
        'pythonExecutable': escapePath(sys.executable),
        'buildPy': escapePath(os.path.join(scriptDir, 'build.py')),
        'platform': escape(platform),
        'compiler': escape(compiler),
        'projectPath': escapePath(projectPath),
        'projectName': escape(project.name),
        'defines': defines,
        'projectSourceFiles': projectSourceFiles,
        'librarySourceFiles': librarySourceFiles,
        'projectIncludeDirectories': projectIncludeDirectories,
        'libraryIncludeDirectories': libraryIncludeDirectories,
    }

    templateFile = os.path.join(scriptDir, 'CMakeLists.txt')
    with open(templateFile, 'rb') as fileHandle:
        template = fileHandle.read()

    cmakelists = \
        '##########################################\n' +\
        '# THIS IS A GENERATED FILE. DO NOT EDIT! #\n' +\
        '##########################################\n' +\
        '\n' +\
        string.Template(template).substitute(variables);

    utility.writeFile(os.path.join(outputDirectory, 'CMakeLists.txt'), cmakelists)

def runCMakeGenerate(projectPath, outputDirectory, platform, compiler, project):
    os.chdir(outputDirectory)
    subprocess.check_call('cmake .', shell=True)

def runCMakeBuild(projectPath, outputDirectory, platform, compiler, project):
    os.chdir(outputDirectory)
    subprocess.check_call('cmake --build .', shell=True)
