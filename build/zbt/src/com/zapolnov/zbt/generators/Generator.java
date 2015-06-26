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

import com.zapolnov.zbt.generators.CMakeGenerator.Target;
import com.zapolnov.zbt.project.Project;
import com.zapolnov.zbt.utility.Utility;
import java.io.File;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Set;

public abstract class Generator
{
    public interface Factory {
        Generator create();
    }

    public abstract void generate(File targetPath, Project project);

    private static Map<String, Map<String, Factory>> allGenerators;
    public static Map<String, Map<String, Factory>> allGenerators()
    {
        if (allGenerators == null) {
            Map<String, Map<String, Factory>> g = new LinkedHashMap<>();
            if (Utility.IS_WINDOWS) {
                Map<String, Factory> win32 = new LinkedHashMap<>();
                win32.put("MinGW Makefiles",            () -> new CMakeGenerator(Target.MINGW32_MAKEFILES));
                win32.put("MSYS Makefiles",             () -> new CMakeGenerator(Target.MSYS32_MAKEFILES));
                win32.put("Visual Studio 12 2013",      () -> new CMakeGenerator(Target.VS2013_WIN32));
                win32.put("Visual Studio 14 2015",      () -> new CMakeGenerator(Target.VS2015_WIN32));

                Map<String, Factory> win64 = new LinkedHashMap<>();
                win64.put("MinGW Makefiles",            () -> new CMakeGenerator(Target.MINGW64_MAKEFILES));
                win64.put("MSYS Makefiles",             () -> new CMakeGenerator(Target.MSYS64_MAKEFILES));
                win64.put("Visual Studio 12 2013",      () -> new CMakeGenerator(Target.VS2013_WIN64));
                win64.put("Visual Studio 14 2015",      () -> new CMakeGenerator(Target.VS2015_WIN64));

                Map<String, Factory> winPhone8 = new LinkedHashMap<>();
                winPhone8.put("Visual Studio 12 2013",  () -> new CMakeGenerator(Target.VS2013_WINPHONE));
                winPhone8.put("Visual Studio 14 2015",  () -> new CMakeGenerator(Target.VS2015_WINPHONE));

                Map<String, Factory> winStore8 = new LinkedHashMap<>();
                winStore8.put("Visual Studio 12 2013",  () -> new CMakeGenerator(Target.VS2013_WINPHONE));
                winStore8.put("Visual Studio 14 2015",  () -> new CMakeGenerator(Target.VS2015_WINPHONE));

                g.put("WinAPI x86", win32);
                g.put("WinAPI x86-64", win64);
                g.put("Windows Phone 8.1", winPhone8);
                g.put("Windows Store 8.1", winStore8);
            }
            allGenerators = g;
        }
        return allGenerators;
    }
}
