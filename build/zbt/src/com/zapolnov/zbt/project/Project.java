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

import com.zapolnov.zbt.generators.Generator;
import com.zapolnov.zbt.plugins.Plugin;
import com.zapolnov.zbt.project.parser.AbstractProjectDirectiveVisitor;
import com.zapolnov.zbt.project.parser.ProjectDirective;
import com.zapolnov.zbt.project.parser.ProjectDirectiveList;
import com.zapolnov.zbt.project.parser.ProjectFileParser;
import com.zapolnov.zbt.project.parser.directives.ImportDirective;
import com.zapolnov.zbt.utility.CommandInvoker;
import com.zapolnov.zbt.utility.Database;
import com.zapolnov.zbt.utility.Utility;
import com.zapolnov.zbt.utility.YamlParser;
import java.io.File;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.TreeMap;

public class Project
{
    public interface BuildCompletionListener
    {
        void onBuildFinished(Throwable error);
    }

    public static final String BUILD_DIRECTORY_NAME = ".build";

    private final File outputDirectory;
    private final File projectDirectory;
    private final Database database;
    private final Map<String, ImportDirective> importedModules = new HashMap<>();
    private final Map<String, Plugin> plugins = new HashMap<>();
    private final ProjectDirectiveList directives = new ProjectDirectiveList(null, false);
    private Map<String, String> options = new HashMap<>();

    public Project(File projectDirectory)
    {
        File projectFile = new File(projectDirectory, ProjectFileParser.PROJECT_FILE_NAME);
        this.outputDirectory = new File(projectDirectory, BUILD_DIRECTORY_NAME);
        this.projectDirectory = projectDirectory;

        ProjectFileParser parser = new ProjectFileParser(this);
        parser.parseFile(projectFile);

        Utility.ensureDirectoryExists(outputDirectory);
        Utility.makeDirectoryHidden(outputDirectory);

        database = new Database(outputDirectory);
    }

    public Database database()
    {
        return database;
    }

    public void closeDatabase()
    {
        database.close();
    }

    public File projectDirectory()
    {
        return projectDirectory;
    }

    public File outputDirectory()
    {
        return outputDirectory;
    }

    public String getConfigurationOption(String name)
    {
        return options.get(name);
    }

    public Map<String, String> configurationOptions()
    {
        return Collections.unmodifiableMap(options);
    }

    public ImportDirective getImportedModule(String modulePath)
    {
        return importedModules.get(modulePath);
    }

    public void addImportedModule(ImportDirective directive)
    {
        importedModules.put(directive.modulePath(), directive);
    }

    public ProjectDirectiveList directives()
    {
        return directives;
    }

    public Plugin loadPlugin(String className)
        throws ClassNotFoundException, InstantiationException, IllegalAccessException
    {
        Plugin plugin = plugins.get(className);
        if (plugin != null)
            return plugin;

        Class<?> pluginClass = Class.forName(className);
        plugin = (Plugin)pluginClass.newInstance();

        plugins.put(className, plugin);

        return plugin;
    }

    public void generate(final Generator generator, Map<String, String> options, final CommandInvoker.Printer printer,
        final BuildCompletionListener listener, final boolean build)
    {
        try {
            System.out.println(String.format("Generating project for %s.", generator.name()));

            directives().visitDirectives(new AbstractProjectDirectiveVisitor() {
                @Override public void visitDirective(ProjectDirective directive) {
                    directive.clearCaches();
                }
            });

            this.options = new TreeMap<>(options);
            if (!this.options.isEmpty()) {
                System.out.println("Using options:");

                int length = 0;
                for (Map.Entry<String, String> option : this.options.entrySet())
                    length = Math.max(length, option.getKey().length());

                String format = String.format("  %%-%ds = %%s", length);
                for (Map.Entry<String, String> option : this.options.entrySet())
                    System.out.println(String.format(format, option.getKey(), option.getValue()));

                System.out.println("");
            }

            Thread thread = new Thread(() -> {
                try {
                    generator.generate(this, printer, build);
                    database.commit();
                } catch (Throwable t) {
                    database.rollbackSafe();
                    listener.onBuildFinished(t);
                    return;
                } finally {
                    this.options = new HashMap<>();
                }

                System.out.println("*** PROJECT HAS BEEN SUCCESSFULLY GENERATED ***\n");
                listener.onBuildFinished(null);
            });
            thread.setDaemon(true);
            thread.start();
        } catch (Throwable t) {
            this.options = new HashMap<>();
            database.rollbackSafe();
            throw t;
        }
    }
}
