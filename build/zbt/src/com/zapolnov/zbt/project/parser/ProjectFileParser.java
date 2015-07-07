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
package com.zapolnov.zbt.project.parser;

import com.zapolnov.zbt.plugins.Plugin;
import com.zapolnov.zbt.project.Project;
import com.zapolnov.zbt.project.parser.directives.CMakeUseOpenGLDirective;
import com.zapolnov.zbt.project.parser.directives.CMakeUseQt5Directive;
import com.zapolnov.zbt.project.parser.directives.CustomDirective;
import com.zapolnov.zbt.project.parser.directives.CustomDirectiveWrapper;
import com.zapolnov.zbt.project.parser.directives.DefineDirective;
import com.zapolnov.zbt.project.parser.directives.EnumerationDirective;
import com.zapolnov.zbt.project.parser.directives.GeneratorSelectorDirective;
import com.zapolnov.zbt.project.parser.directives.HeaderPathsDirective;
import com.zapolnov.zbt.project.parser.directives.RootProjectSelectorDirective;
import com.zapolnov.zbt.project.parser.directives.TargetNameDirective;
import com.zapolnov.zbt.project.parser.directives.ImportDirective;
import com.zapolnov.zbt.project.parser.directives.SelectorDirective;
import com.zapolnov.zbt.project.parser.directives.SourceDirectoriesDirective;
import com.zapolnov.zbt.project.parser.directives.ThirdPartyHeaderPathsDirective;
import com.zapolnov.zbt.project.parser.directives.ThirdPartySourceDirectoriesDirective;
import com.zapolnov.zbt.utility.Utility;
import com.zapolnov.zbt.utility.YamlParser;
import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public final class ProjectFileParser
{
    public static final String PROJECT_FILE_NAME = "project.yml";

    private static final Pattern ENUM_REG_EXP = Pattern.compile(String.format("^\\^(%s)\\((%s(,%s)*)\\)$",
        EnumerationDirective.NAME_PATTERN, EnumerationDirective.VALUE_PATTERN, EnumerationDirective.VALUE_PATTERN));
    private static final Pattern EXE_NAME_REG_EXP = Pattern.compile(String.format("^%s$",
        TargetNameDirective.PATTERN));

    private final Project project;
    private final Set<String> moduleImportStack = new LinkedHashSet<>();
    private final Set<Plugin> plugins = new LinkedHashSet<>();

    public ProjectFileParser(Project project)
    {
        this.project = project;
    }

    public void parseFile(File file)
    {
        parseFile(project.directives(), file);
    }

    private void parseFile(ProjectDirectiveList directiveList, File file)
    {
        YamlParser.Option root = YamlParser.readFile(file);
        if (root == null)
            return;

        String moduleName = Utility.getCanonicalPath(file);
        if (!moduleImportStack.contains(moduleName)) {
            moduleImportStack.add(moduleName);
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
            } finally {
                moduleImportStack.remove(moduleName);
            }
        }
    }

    private void processOptions(File basePath, ProjectDirectiveList directiveList,
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
            case '^': directive = processSelector(basePath, directiveList, key, keyOption, valueOption); break;

            default:
                switch (key)
                {
                case "+generator": directive = processGeneratorSelector(basePath, directiveList, valueOption); break;
                case "+if(root_project)": directive = processRootProjectSelector(basePath, directiveList, valueOption); break;
                case "enum": directive = processEnum(directiveList, keyOption, valueOption); break;
                case "import": directive = processImport(basePath, directiveList, valueOption); break;
                case "define": directive = processDefine(valueOption); break;
                case "source_directories": directive = processSourceDirectories(basePath, valueOption); break;
                case "3rdparty_source_directories": directive = processThirdPartySourceDirectories(basePath, valueOption); break;
                case "header_search_paths": directive = processHeaderSearchPaths(basePath, valueOption); break;
                case "3rdparty_header_search_paths": directive = processThirdPartyHeaderSearchPaths(basePath, valueOption); break;
                case "target_name": directive = processTargetName(valueOption); break;
                case "cmake-use-opengl": directive = processCMakeUseOpenGL(valueOption); break;
                case "cmake-use-qt5": directive = processCMakeUseQt5(valueOption); break;
                case "plugin": processPlugin(valueOption); break;
                default:
                    for (Plugin plugin : plugins) {
                        CustomDirective pluginDirective =
                            plugin.processDirective(project, basePath, key, keyOption, valueOption);
                        if (pluginDirective != null) {
                            directive = new CustomDirectiveWrapper(pluginDirective);
                            break;
                        }
                    }
                    if (directive == null)
                        throw new YamlParser.Error(keyOption, String.format("Unknown option \"%s\".", key));
                }
            }

            if (directive != null)
                directiveList.addDirective(directive);
        }
    }

    private ProjectDirective processSelector(File basePath, ProjectDirectiveList directiveList,
        String key, YamlParser.Option keyOption, YamlParser.Option valueOption)
    {
        Matcher matcher = ENUM_REG_EXP.matcher(key);
        if (!matcher.matches() || matcher.groupCount() != 3)
            throw new YamlParser.Error(keyOption, "Invalid selector.");

        if (!valueOption.isMapping())
            throw new YamlParser.Error(valueOption, "Expected mapping.");

        String enumID = matcher.group(1);

        Set<String> matchingValues = new LinkedHashSet<>();
        Collections.addAll(matchingValues, matcher.group(2).split(","));

        ProjectDirectiveList innerDirectives = new ProjectDirectiveList(directiveList, false);
        processOptions(basePath, innerDirectives, valueOption.toMapping());

        return new SelectorDirective(enumID, matchingValues, innerDirectives);
    }

    private ProjectDirective processGeneratorSelector(File basePath, ProjectDirectiveList directiveList,
        YamlParser.Option valueOption)
    {
        if (!valueOption.isMapping())
            throw new YamlParser.Error(valueOption, "Expected mapping.");

        Map<String, ProjectDirectiveList> mapping = new HashMap<>();
        for (Map.Entry<YamlParser.Option, YamlParser.Option> item : valueOption.toMapping().entrySet()) {
            String key = item.getKey().toString();
            if (key == null)
                throw new RuntimeException("Expected string.");

            if (!key.startsWith("+"))
                throw new RuntimeException("Keys in generator selector should begin with '+'.");

            String name = key.substring(1);
            if (!item.getValue().isMapping())
                throw new YamlParser.Error(item.getValue(), "Expected mapping.");

            ProjectDirectiveList innerDirectives = new ProjectDirectiveList(directiveList, false);
            processOptions(basePath, innerDirectives, item.getValue().toMapping());

            if (mapping.containsKey(name))
                throw new YamlParser.Error(item.getKey(), String.format("Duplicate key \"%s\".", name));
            mapping.put(name, innerDirectives);
        }

        return new GeneratorSelectorDirective(mapping);
    }

    private ProjectDirective processRootProjectSelector(File basePath, ProjectDirectiveList directiveList,
        YamlParser.Option valueOption)
    {
        if (!valueOption.isMapping())
            throw new YamlParser.Error(valueOption, "Expected mapping.");

        ProjectDirectiveList innerDirectives = new ProjectDirectiveList(directiveList, false);
        processOptions(basePath, innerDirectives, valueOption.toMapping());

        return new RootProjectSelectorDirective(innerDirectives, moduleImportStack.size() == 1);
    }

    private ProjectDirective processEnum(ProjectDirectiveList directiveList,
        YamlParser.Option keyOption, YamlParser.Option valueOption)
    {
        if (!valueOption.isMapping())
            throw new YamlParser.Error(valueOption, "Expected mapping.");

        String defaultValue = null;
        String id = null;
        String title = null;
        Map<String, String> values = new LinkedHashMap<>();
        YamlParser.Option defaultValueOption = null;

        for (Map.Entry<YamlParser.Option, YamlParser.Option> item : valueOption.toMapping().entrySet()) {
            YamlParser.Option subKeyOption = item.getKey();
            YamlParser.Option subValueOption = item.getValue();

            String subKey = subKeyOption.toString();
            if (subKey == null)
                throw new YamlParser.Error(subKeyOption, "Mapping key should be a string.");

            switch (subKey)
            {
            case "id":
                id = subValueOption.toString();
                if (id == null)
                    throw new YamlParser.Error(subValueOption, "Expected string.");
                if (!EnumerationDirective.NAME_PATTERN.matcher(id).matches())
                    throw new YamlParser.Error(subValueOption, String.format("Invalid enumeration id \"%s\".", id));
                if (!directiveList.reserveEnumerationID(id))
                    throw new YamlParser.Error(subValueOption, String.format("Duplicate enumeration id \"%s\".", id));
                break;

            case "title":
                title = subValueOption.toString();
                if (title == null)
                    throw new YamlParser.Error(subValueOption, "Expected string.");
                if (title.length() == 0)
                    throw new YamlParser.Error(subValueOption, "Title should not be empty.");
                break;

            case "default":
                defaultValue = subValueOption.toString();
                defaultValueOption = subValueOption;
                if (defaultValue == null)
                    throw new YamlParser.Error(subValueOption, "Expected string.");
                break;

            case "values":
                processEnumValues(values, subValueOption);
                break;

            default:
                throw new YamlParser.Error(subKeyOption, String.format("Unknown option \"%s\".", subKey));
            }
        }

        if (id == null)
            throw new YamlParser.Error(keyOption, "Missing enumeration id.");
        if (title == null)
            throw new YamlParser.Error(keyOption, "Missing enumeration title.");
        if (values.isEmpty())
            throw new YamlParser.Error(keyOption, "Missing enumeration values.");

        if (defaultValue != null && !values.containsKey(defaultValue))
            throw new YamlParser.Error(defaultValueOption, String.format("Invalid default value \"%s\".", defaultValue));

        return new EnumerationDirective(id, title, defaultValue, values);
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

    private ProjectDirective processImport(File basePath, ProjectDirectiveList directiveList,
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

        ImportDirective importDirective = null;

        for (YamlParser.Option module : modules) {
            String name = module.toString();
            if (name == null)
                throw new YamlParser.Error(module, "Expected string.");

            File moduleDirectory = new File(basePath, name);
            File moduleFile = new File(moduleDirectory, PROJECT_FILE_NAME);
            if (!moduleFile.exists()) {
                String fileName = Utility.getCanonicalPath(moduleFile);
                throw new YamlParser.Error(module, String.format("File \"%s\" does not exist.", fileName));
            }

            if (importDirective != null)
                directiveList.addDirective(importDirective);

            String modulePath =  Utility.getCanonicalPath(moduleDirectory);
            importDirective = project.getImportedModule(modulePath);
            if (importDirective == null) {
                ProjectDirectiveList innerDirectiveList = new ProjectDirectiveList(directiveList, true);
                parseFile(innerDirectiveList, moduleFile);

                importDirective = new ImportDirective(modulePath, innerDirectiveList);
                project.addImportedModule(importDirective);
            }
        }

        return importDirective;
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

    private ProjectDirective processThirdPartySourceDirectories(File basePath, YamlParser.Option valueOption)
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

        return new ThirdPartySourceDirectoriesDirective(directories);
    }

    private ProjectDirective processHeaderSearchPaths(File basePath, YamlParser.Option valueOption)
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

        return new HeaderPathsDirective(directories);
    }

    private ProjectDirective processThirdPartyHeaderSearchPaths(File basePath, YamlParser.Option valueOption)
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

        return new ThirdPartyHeaderPathsDirective(directories);
    }

    private ProjectDirective processTargetName(YamlParser.Option valueOption)
    {
        String name = valueOption.toString();
        if (name == null)
            throw new YamlParser.Error(valueOption, "Expected string.");

        Matcher matcher = EXE_NAME_REG_EXP.matcher(name);
        if (!matcher.matches())
            throw new YamlParser.Error(valueOption, "Invalid target name.");

        return new TargetNameDirective(name);
    }

    private ProjectDirective processCMakeUseOpenGL(YamlParser.Option valueOption)
    {
        String name = valueOption.toString();

        boolean value;
        if ("true".equals(name))
            value = true;
        else if ("false".equals(name))
            value = false;
        else
            throw new YamlParser.Error(valueOption, "Expected 'true' or 'false'.");

        return new CMakeUseOpenGLDirective(value);
    }

    private ProjectDirective processCMakeUseQt5(YamlParser.Option valueOption)
    {
        String name = valueOption.toString();

        boolean value;
        if ("true".equals(name))
            value = true;
        else if ("false".equals(name))
            value = false;
        else
            throw new YamlParser.Error(valueOption, "Expected 'true' or 'false'.");

        return new CMakeUseQt5Directive(value);
    }

    private void processPlugin(YamlParser.Option valueOption)
    {
        String className = valueOption.toString();
        if (className == null)
            throw new YamlParser.Error(valueOption, "Expected string.");

        try {
            Plugin plugin = project.loadPlugin(className);
            plugins.add(plugin);
        } catch (ClassNotFoundException|InstantiationException|IllegalAccessException e) {
            throw new YamlParser.Error(valueOption, e);
        }
    }
}
