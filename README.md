Game Engine
===========

Building
--------

You will need the following software to build samples:

  * *CMake* 3.2.2 or newer
  * *Python* 2.7.10 or newer

There is a script `build-samples.py` in the root directory of the project.
If you invoke it without options, it will build samples for the current platform
(on Windows it will assume MinGW is installed).

Release build is assumed by default. To build a debug version pass option `-c Debug`
to `build-samples.py`.

Code should successfully compile with any C++11-conforming compiler targeting one of
the supported platforms. The following target platforms and compilers have been tested:

  - Microsoft Windows (Win32)

    - MinGW 4.9.1 or newer (pass option `-t win32` or `-t win64` to `build-samples.py`
      to select 32-bit or 64-bit build respectively)

    - Visual Studio 2013 (pass option `-t win32-msvc` or `-t win64-msvc` to `build-samples.py`
      to select 32-bit or 64-bit build respectively)

  - Microsoft Windows (WinRT)

    - **INCOMPLETE** Visual Studio 2013 targeting *Windows Store 8.1* (pass option `-t winstore81` or `-t winstore81-x64`
      to `build-samples.py` to select 32-bit or 64-bit build respectively)

  - GNU/Linux

    - GCC 4.9.2 on Ubuntu 15.04 (pass option `-t linux32` or `-t linux64` to `build-samples.py`
      to select 32-bit or 64-bit build respectively)

  - Apple OS X

    - GCC 5.2.0 on OSX 10.6.8 Snow Leopard (option `-t osx` to `build-samples.py`)

  - iOS

    - **NOT YET IMPLEMENTED** (option `-t ios` to `build-samples.py`)

  - Android

    - **NOT YET IMPLEMENTED** (option `-t android` to `build-samples.py`)

  - Windows Phone

    - **INCOMPLETE** Visual Studio 2013 (option `-t winphone81`)

  - Google Native Client (PNaCl, Google Chrome)

    - **NOT YET IMPLEMENTED** (option `-t pnacl` to `build-samples.py`)

  - HTML5

    - **NOT YET IMPLEMENTED** (option `-t emscripten` to `build-samples.py`)

You may invoke script `build-samples.py` with option `--help` to get a full
list of supported command line options.

### Windows

You will need *Visual Studio 2013* or *MinGW* to build on Windows.

Run script `build-samples.py`

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
