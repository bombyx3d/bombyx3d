Game Engine
===========

Building samples
----------------

You will need the following software to build samples:

  * *CMake 3.2.2* or newer
  * *Python 2.7.10* or newer

Code should successfully compile with any C++11-conforming compiler targeting one of
the supported platforms, although only a subset of compiler/platform combinations have been tested.

There is a script `build-samples.py` in the root directory of the project.
If you invoke it without options, it will build *Release* version of samples
for the current platform (only Windows, Linux and OSX are supported as host platforms).

Use option `-c Debug` to build *Debug* version and option `-t <target>` to specify
another target platform. You may invoke script `build-samples.py` with option `--help` to get
a full list of supported command line options.

The following target platforms and compilers have been tested:

  - *Microsoft Windows* (*x86* and *amd64*)
    - *MinGW-w64 4.9.3* (option `-t win32`)
    - **INCOMPLETE** (option `-t win64`)
    - *Visual Studio 2013* (options `-t win32-msvc` and `-t win64-msvc`)
    - **INCOMPLETE** *Visual Studio 2013* targeting *Windows Store* (options `-t winstore81` and `-t winstore81-x64`)

  - *GNU/Linux* (*x86* and *amd64*)
    - *GCC 4.9.2* on *Ubuntu 15.04* (options `-t linux32` and `-t linux64`)

  - *Apple OS X 10.6 (Snow Leopard)* (*amd64*)
    - *GCC 5.2.0* on *OSX 10.6.8* Snow Leopard (option `-t osx`)

  - *iOS*
    - **NOT YET IMPLEMENTED** (option `-t ios`)

  - *Google Android*
    - **NOT YET IMPLEMENTED** (option `-t android`)

  - *Microsoft Windows Phone 8.1*
    - **INCOMPLETE** *Visual Studio 2013* (option `-t winphone81`)

  - *Google Native Client*
    - **NOT YET IMPLEMENTED** (option `-t pnacl`)

  - *HTML5*
    - **NOT YET IMPLEMENTED** (option `-t emscripten`)


Building tools
--------------

You will need the following software to build samples:

  * *CMake 3.2.2* or newer
  * *Python 2.7.10* or newer
  * *Qt 5.4* or newer

Code should successfully compile with any C++11-conforming compiler targeting
Microsoft Windows, Linux or OSX.

There is a script `build-tools.py` in the root directory of the project.
If you invoke it without options, it will build *Release* version of tools
for the current platform.

Use option `-c Debug` to build *Debug* version. You may invoke script `build-tools.py`
with option `--help` to get a full list of supported command line options.


License
-------

All my code is licensed under the MIT license.

Third party libraries and assets are licensed under their own licenses.
Please consult files *LICENSE.txt*, *README.txt* and others in the corresponding
directories.

```
Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com).

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```
