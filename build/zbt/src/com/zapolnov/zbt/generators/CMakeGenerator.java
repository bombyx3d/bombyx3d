/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
package com.zapolnov.zbt.generators;

import com.zapolnov.zbt.project.Project;
import java.io.File;

public class CMakeGenerator extends Generator
{
    public enum Target
    {
        // Windows
        MINGW32_MAKEFILES,
        MSYS32_MAKEFILES,
        MINGW64_MAKEFILES,
        MSYS64_MAKEFILES,
        VS2013_WIN32,
        VS2013_WIN64,
        VS2013_WINPHONE,
        VS2013_WINSTORE,
        VS2015_WIN32,
        VS2015_WIN64,
        VS2015_WINPHONE,
        VS2015_WINSTORE,
    }

    private final Target target;

    public CMakeGenerator(Target target)
    {
        this.target = target;
    }

    @Override public void generate(File targetPath, Project project)
    {
    }

    public void writeCMakeLists(File targetPath, Project project)
    {
    }
}
