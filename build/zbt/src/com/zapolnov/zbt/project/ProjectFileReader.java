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
package com.zapolnov.zbt.project;

import com.zapolnov.zbt.project.directive.DefineDirective;
import com.zapolnov.zbt.project.directive.EnumerationDirective;
import com.zapolnov.zbt.project.directive.SelectorDirective;
import com.zapolnov.zbt.project.directive.SourceDirectoriesDirective;
import com.zapolnov.zbt.utility.Utility;
import com.zapolnov.zbt.utility.YamlParser;
import java.io.File;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public final class ProjectFileReader
{
    public static final String PROJECT_FILE_NAME = "project.yml";

    private final Project project;

    public ProjectFileReader(Project project)
    {
        this.project = project;
    }

    public void readFile(File file)
    {
        readFile(project.directives(), file);
    }

    private void readFile(List<ProjectDirective> directiveList, File file)
    {
        YamlParser.Option root = YamlParser.readFile(file);
        if (root == null)
            return;

        try {
            if (!root.isMapping())
                throw new YamlParser.Error(root, "Expected mapping at the root level.");
            processOptions(file.getAbsoluteFile().getParentFile(), directiveList, root.toMapping());
        } catch (YamlParser.Error e) {
            throw e;
        } catch (Throwable t) {
            String fileName = Utility.getCanonicalPath(file);
            String msg = Utility.getExceptionMessage(t);
            throw new RuntimeException(String.format("Unable to parse YAML file \"%s\".\nError: %s", fileName, msg), t);
        }
    }

    private static final Pattern enumRegExp = Pattern.compile(String.format("\\^(%s)\\((%s)\\)$",
        EnumerationDirective.NAME_PATTERN, EnumerationDirective.VALUE_PATTERN));

    private void processOptions(File basePath, List<ProjectDirective> directiveList,
        Map<YamlParser.Option, YamlParser.Option> options)
    {
        for (Map.Entry<YamlParser.Option, YamlParser.Option> item : options.entrySet()) {
            YamlParser.Option keyOption = item.getKey();
            YamlParser.Option valueOption = item.getValue();

            String key = keyOption.toString();
            if (key == null)
                throw new YamlParser.Error(keyOption, "Mapping key should be a string.");

            ProjectDirective directive = null;

            char firstChar = (key.length() > 0 ? key.charAt(0) : 0);
            switch (firstChar)
            {
            case '^': directive = processSelector(basePath, key, keyOption, valueOption); break;

            default:
                switch (key)
                {
                case "enum": directive = processEnum(keyOption, valueOption); break;
                case "import": processImport(basePath, directiveList, valueOption); break;
                case "define": directive = processDefine(valueOption); break;
                case "source_directories": directive = processSourceDirectories(basePath, valueOption); break;
                default: throw new YamlParser.Error(keyOption, String.format("Unknown option \"%s\".", key));
                }
            }

            if (directive != null)
                directiveList.add(directive);
        }
    }

    private ProjectDirective processSelector(File basePath, String key,
        YamlParser.Option keyOption, YamlParser.Option valueOption)
    {
        Matcher matcher = enumRegExp.matcher(key);
        if (!matcher.find() || matcher.groupCount() != 2)
            throw new YamlParser.Error(keyOption, "Invalid selector.");

        if (!valueOption.isMapping())
            throw new YamlParser.Error(valueOption, "Expected mapping.");

        String enumName = matcher.group(1);
        String enumValue = matcher.group(2);

        List<ProjectDirective> innerDirectives = new ArrayList<>();
        processOptions(basePath, innerDirectives, valueOption.toMapping());

        return new SelectorDirective(enumName, enumValue, innerDirectives);
    }

    private ProjectDirective processEnum(YamlParser.Option keyOption, YamlParser.Option valueOption)
    {
        if (!valueOption.isMapping())
            throw new YamlParser.Error(valueOption, "Expected mapping.");

        String name = null;
        String description = null;
        Map<String, String> values = new LinkedHashMap<>();

        for (Map.Entry<YamlParser.Option, YamlParser.Option> item : valueOption.toMapping().entrySet()) {
            YamlParser.Option subKeyOption = item.getKey();
            YamlParser.Option subValueOption = item.getValue();

            String subKey = subKeyOption.toString();
            if (subKey == null)
                throw new YamlParser.Error(subKeyOption, "Mapping key should be a string.");

            switch (subKey)
            {
            case "name":
                name = subValueOption.toString();
                if (name == null)
                    throw new YamlParser.Error(subValueOption, "Expected string.");
                if (!EnumerationDirective.NAME_PATTERN.matcher(name).matches())
                    throw new YamlParser.Error(subValueOption, String.format("Invalid enumeration name \"%s\".", name));
                if (!project.addEnumerationName(name))
                    throw new YamlParser.Error(subValueOption, String.format("Duplicate enumeration name \"%s\".", name));
                break;

            case "description":
                description = subValueOption.toString();
                if (description == null)
                    throw new YamlParser.Error(subValueOption, "Expected string.");
                break;

            case "values":
                processEnumValues(values, subValueOption);
                break;

            default:
                throw new YamlParser.Error(subKeyOption, String.format("Unknown option \"%s\".", subKey));
            }
        }

        if (name == null)
            throw new YamlParser.Error(keyOption, "Missing enumeration name.");
        if (description == null)
            throw new YamlParser.Error(keyOption, "Missing enumeration description.");
        if (values.isEmpty())
            throw new YamlParser.Error(keyOption, "Missing enumeration values.");

        return new EnumerationDirective(name, description, values);
    }

    private void processEnumValues(Map<String, String> values, YamlParser.Option valueOption)
    {
        if (!valueOption.isMapping())
            throw new YamlParser.Error(valueOption, "Expected mapping.");

        for (Map.Entry<YamlParser.Option, YamlParser.Option> value : valueOption.toMapping().entrySet()) {
            YamlParser.Option nameOption = value.getKey();
            YamlParser.Option descriptionOption = value.getValue();

            String name = nameOption.toString();
            if (name == null)
                throw new YamlParser.Error(nameOption, "Mapping key should be a string.");

            String description = descriptionOption.toString();
            if (description == null)
                throw new YamlParser.Error(descriptionOption, "Expected string.");

            if (!EnumerationDirective.VALUE_PATTERN.matcher(name).matches())
                throw new YamlParser.Error(nameOption, String.format("Invalid enumeration value \"%s\".", name));

            if (values.get(name) != null)
                throw new YamlParser.Error(nameOption, String.format("Duplicate enumeration value \"%s\".", name));
            values.put(name, description);
        }
    }

    private void processImport(File basePath, List<ProjectDirective> directiveList,
        YamlParser.Option valueOption)
    {
        List<YamlParser.Option> modules;
        if (valueOption.isSequence())
            modules = valueOption.toSequence();
        else {
            if (valueOption.toString() == null)
                throw new YamlParser.Error(valueOption, "Expected string or sequence of strings.");
            modules = new ArrayList<>(1);
            modules.add(valueOption);
        }

        for (YamlParser.Option module : modules) {
            String name = module.toString();
            if (name == null)
                throw new YamlParser.Error(module, "Expected string.");

            File file = new File(new File(basePath, name), PROJECT_FILE_NAME);
            if (!file.exists()) {
                String fileName = Utility.getCanonicalPath(file);
                throw new YamlParser.Error(module, String.format("File \"%s\" does not exist.", fileName));
            }

            readFile(directiveList, file);
        }
    }

    private ProjectDirective processDefine(YamlParser.Option valueOption)
    {
        List<YamlParser.Option> defines;
        if (valueOption.isSequence())
            defines = valueOption.toSequence();
        else {
            if (valueOption.toString() == null)
                throw new YamlParser.Error(valueOption, "Expected string or sequence of strings.");
            defines = new ArrayList<>(1);
            defines.add(valueOption);
        }

        List<String> defineList = new ArrayList<>();
        for (YamlParser.Option define : defines) {
            String name = define.toString();
            if (name == null)
                throw new YamlParser.Error(define, "Expected string.");
            defineList.add(name);
        }

        return new DefineDirective(defineList);
    }

    private ProjectDirective processSourceDirectories(File basePath, YamlParser.Option valueOption)
    {
        List<YamlParser.Option> paths;
        if (valueOption.isSequence())
            paths = valueOption.toSequence();
        else {
            if (valueOption.toString() == null)
                throw new YamlParser.Error(valueOption, "Expected string or sequence of strings.");
            paths = new ArrayList<>(1);
            paths.add(valueOption);
        }

        List<File> directories = new ArrayList<>();
        for (YamlParser.Option path : paths) {
            String name = path.toString();
            if (name == null)
                throw new YamlParser.Error(path, "Expected string.");

            File file = new File(basePath, name);
            if (!file.exists()) {
                String fileName = Utility.getCanonicalPath(file);
                throw new YamlParser.Error(path, String.format("Directory \"%s\" does not exist.", fileName));
            }
            if (!file.isDirectory()) {
                String fileName = Utility.getCanonicalPath(file);
                throw new YamlParser.Error(path, String.format("\"%s\" is not a directory.", fileName));
            }

            directories.add(Utility.getCanonicalFile(file));
        }

        return new SourceDirectoriesDirective(directories);
    }
}
