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
package com.zapolnov.zbt;

import com.zapolnov.zbt.utility.Utility;
import com.zapolnov.zbt.utility.YamlParser;
import java.io.File;
import java.util.Map;

public final class ProjectFileReader
{
    public void readFile(File file)
    {
        YamlParser.Option root = YamlParser.readFile(file);

        try {
            if (root == null || !root.isMapping())
                throw new YamlParser.Error(root, "Expected mapping at the root level.");

            for (Map.Entry<YamlParser.Option, YamlParser.Option> obj : root.toMapping().entrySet())
                System.out.println(obj.getKey());
        } catch (YamlParser.Error e) {
            throw e;
        } catch (Throwable t) {
            String fileName = Utility.getCanonicalPath(file);
            String msg = Utility.getExceptionMessage(t);
            throw new RuntimeException(String.format("Unable to parse YAML file \"%s\".\nError: %s", fileName, msg), t);
        }
    }

    private void processOptions(Map<String, Object> options)
    {
    }
}
