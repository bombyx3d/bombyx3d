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

import os
import yaml
from utility import BuildError

PROJECT_FILE_NAME = 'project.yml'

class Project:
    def __init__(self, name = 'Project'):
        self.name = name
        self.defines = []
        self.projectSources = []
        self.librarySources = []
        self.projectIncludeDirectories = []
        self.libraryIncludeDirectories = []

##############################################################################

class ProjectReader:
    def __init__(self):
        self._isRootProject = True
        self._project = Project()
        pass

    def _yamlString(self, value):
        return str(value)

    def _yamlSequence(self, value):
        if isinstance(value, list):
            return value
        else:
            return [ value ]

    def _enumerateFiles(self, value):
        files = []
        for name in self._yamlSequence(value):
            path = os.path.join(self._projectPath, name)
            if os.path.isfile(path):
                files.append(os.path.abspath(path))
            elif os.path.isdir(path):
                for root, dirnames, filenames in os.walk(path):
                    for filename in filenames:
                        files.append(os.path.abspath(os.path.join(root, filename)))
        return files

    #############################

    def _readDefine(self, value):
        for name in self._yamlSequence(value):
            self._project.defines.append(name)

    def _readImport(self, value):
        for name in self._yamlSequence(value):
            isRootProject = self._isRootProject
            try:
                self._isRootProject = False
                self.read(os.path.join(self._projectPath, name))
            finally:
                self._isRootProject = isRootProject

    def _readProjectName(self, value):
        if self._isRootProject:
            self._project.name = self._yamlString(value)

    def _readProjectSources(self, value):
        self._project.projectSources.extend(self._enumerateFiles(value))

    def _readLibrarySources(self, value):
        self._project.librarySources.extend(self._enumerateFiles(value))

    def _readProjectIncludeDirectories(self, value):
        for name in self._yamlSequence(value):
            path = os.path.join(self._projectPath, name)
            self._project.projectIncludeDirectories.append(path)

    def _readLibraryIncludeDirectories(self, value):
        for name in self._yamlSequence(value):
            path = os.path.join(self._projectPath, name)
            self._project.libraryIncludeDirectories.append(path)

    #############################

    def read(self, projectPath):
        if projectPath =='.':
            projectFile = PROJECT_FILE_NAME
        else:
            projectFile = os.path.join(projectPath, PROJECT_FILE_NAME)
        self._projectPath = projectPath

        # Check that project file exists and is a file
        if not os.path.exists(projectFile):
            raise BuildError('File does not exist: "%s".' % projectFile)
        elif not os.path.isfile(projectFile):
            raise BuildError('Not a file: "%s".' % projectFile)

        # Parse the project file
        print('Reading project file %s' % projectFile)
        with open(projectFile) as fileHandle:

            options = {
                'Define': self._readDefine,
                'Import': self._readImport,
                'ProjectName': self._readProjectName,
                'ProjectSources': self._readProjectSources,
                'LibrarySources': self._readLibrarySources,
                'ProjectIncludeDirectories': self._readProjectIncludeDirectories,
                'LibraryIncludeDirectories': self._readLibraryIncludeDirectories,
            }

            config = yaml.load(fileHandle)
            for name in config:
                if not name in options:
                    raise BuildError('Invalid option "%s" in project file "%s".' % (name, projectFile))
                options[name](config[name])

        return self._project
