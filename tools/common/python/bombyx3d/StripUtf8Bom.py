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
import fnmatch
import sys
import codecs

def stripUtf8BomFromFile(file):
    BUFSIZE = 4096
    BOMLEN = len(codecs.BOM_UTF8)

    with open(file, "r+b") as fp:
        chunk = fp.read(BUFSIZE)
        if chunk.startswith(codecs.BOM_UTF8):
            print("Stripping UTF8 BOM from %s" % file);
            i = 0
            chunk = chunk[BOMLEN:]
            while chunk:
                fp.seek(i)
                fp.write(chunk)
                i += len(chunk)
                fp.seek(BOMLEN, os.SEEK_CUR)
                chunk = fp.read(BUFSIZE)
            fp.seek(-BOMLEN, os.SEEK_CUR)
            fp.truncate()

def stripUtf8BomFromDirectory(path):
    exclude = set([ '.git', '.svn', 'cmake-build' ])
    for root, dirnames, filenames in os.walk(path, topdown=True):
        dirnames[:] = [d for d in dirnames if d not in exclude]
        for pattern in [
            '*.c',
            '*.cpp',
            '*.h',
            '*.hpp',
            '*.inl',
            '*.lua',
            '*.java',
            '*.cs',
            '*.dox',
            '*.bat',
            '*.cmd',
            '*.sh',
            '*.py',
            '*.xml',
            '*.yml',
            '*.md',
            '*.js',
            '*.json',
            '*.txt',
            '*.material',
            '*.glsl',
            '*.cmake',
            '*.xaml',
            '.gitignore',
            '.gitattributes',
            'Doxyfile'
        ]:
            for filename in fnmatch.filter(filenames, pattern):
                stripUtf8BomFromFile(os.path.join(root, filename))
