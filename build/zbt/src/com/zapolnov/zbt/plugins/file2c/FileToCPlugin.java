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
package com.zapolnov.zbt.plugins.file2c;

import com.zapolnov.zbt.plugins.Plugin;
import com.zapolnov.zbt.project.Project;
import com.zapolnov.zbt.project.parser.directives.CustomDirective;
import com.zapolnov.zbt.utility.Utility;
import com.zapolnov.zbt.utility.YamlParser;
import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class FileToCPlugin extends Plugin
{
    @Override public CustomDirective processDirective(Project project, File basePath, String key,
        YamlParser.Option keyOption, YamlParser.Option valueOption)
    {
        if ("file2c".equals(key)) {
            CompressionMethod compressionMethod = CompressionMethod.NONE;
            File input = null;
            String output = null;
            String identifier = null;
            String namespace = null;

            if (!valueOption.isMapping())
                throw new YamlParser.Error(valueOption, "Expected mapping.");

            for (Map.Entry<YamlParser.Option, YamlParser.Option> item : valueOption.toMapping().entrySet()) {
                YamlParser.Option subKeyOption = item.getKey();
                YamlParser.Option subValueOption = item.getValue();
                String subValue;

                String subKey = subKeyOption.toString();
                if (subKey == null)
                    throw new YamlParser.Error(subKeyOption, "Mapping key should be a string.");

                switch (subKey)
                {
                case "input":
                    subValue = subValueOption.toString();
                    if (subValue == null)
                        throw new YamlParser.Error(subValueOption, "Expected string.");
                    input = new File(basePath, subValue);
                    if (!input.exists()) {
                        throw new YamlParser.Error(subValueOption,
                            String.format("File \"%s\" does not exist.", Utility.getCanonicalPath(input)));
                    }
                    input = Utility.getCanonicalFile(input);
                    break;

                case "output":
                    subValue = subValueOption.toString();
                    if (subValue == null)
                        throw new YamlParser.Error(subValueOption, "Expected string.");
                    if (subValue.length() == 0)
                        throw new YamlParser.Error(subValueOption, "Expected file name.");
                    output = subValue;
                    break;

                case "identifier":
                    subValue = subValueOption.toString();
                    if (subValue == null)
                        throw new YamlParser.Error(subValueOption, "Expected string.");
                    if (subValue.length() == 0)
                        throw new YamlParser.Error(subValueOption, "Expected identifier.");
                    identifier = subValue;
                    break;

                case "namespace":
                    subValue = subValueOption.toString();
                    if (subValue == null)
                        throw new YamlParser.Error(subValueOption, "Expected string.");
                    if (subValue.length() == 0)
                        throw new YamlParser.Error(subValueOption, "Expected identifier.");
                    namespace = subValue;
                    break;

                case "compress":
                    subValue = subValueOption.toString();
                    if (subValue == null)
                        throw new YamlParser.Error(subValueOption, "Expected string.");
                    compressionMethod = null;
                    for (CompressionMethod method : CompressionMethod.values()) {
                        if (method.name.equals(subValue)) {
                            compressionMethod = method;
                            break;
                        }
                    }
                    if (compressionMethod == null) {
                        List<String> validValues = new ArrayList<>();
                        for (CompressionMethod method : CompressionMethod.values())
                            validValues.add(method.name);
                        throw new YamlParser.Error(subValueOption, String.format(
                            "Invalid compression method. Valid values are: \"%s\".",
                            String.join("\", \"", validValues)));
                    }
                    break;

                default:
                    throw new YamlParser.Error(subKeyOption, String.format("Unknown option \"%s\".", subKey));
                }
            }

            if (input == null)
                throw new YamlParser.Error(keyOption, "Missing input file name.");
            if (output == null)
                throw new YamlParser.Error(keyOption, "Missing output file name.");
            if (identifier == null)
                throw new YamlParser.Error(keyOption, "Missing identifier name.");

            return new FileToCDirective(input, output, identifier, namespace, compressionMethod);
        }

        return null;
    }
}
