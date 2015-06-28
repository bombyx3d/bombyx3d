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
import com.zapolnov.zbt.project.parser.ProjectDirectiveList;
import com.zapolnov.zbt.project.parser.ProjectFileParser;
import com.zapolnov.zbt.project.parser.directives.ImportDirective;
import com.zapolnov.zbt.utility.Database;
import com.zapolnov.zbt.utility.Utility;
import java.io.File;
import java.util.HashMap;
import java.util.Map;

public class Project
{
    public static final String BUILD_DIRECTORY_NAME = ".build";

    private final File projectDirectory;
    private final File projectFile;
    private final File outputDirectory;
    private final Database database;
    private final Map<String, String> options = new HashMap<>();
    private final Map<String, ImportDirective> importedModules = new HashMap<>();
    private final ProjectDirectiveList directives = new ProjectDirectiveList(null, false);

    public Project(File projectDirectory)
    {
        this.projectDirectory = projectDirectory;
        this.projectFile = new File(projectDirectory, ProjectFileParser.PROJECT_FILE_NAME);
        this.outputDirectory = new File(projectDirectory, BUILD_DIRECTORY_NAME);

        ProjectFileParser parser = new ProjectFileParser(this);
        parser.parseFile(projectFile);

        if (!outputDirectory.exists()) {
            if (!outputDirectory.mkdirs()) {
                throw new RuntimeException(String.format("Unable to create directory \"%s\".",
                    Utility.getCanonicalPath(outputDirectory)));
            }
        }

        if (!outputDirectory.isDirectory()) {
            throw new RuntimeException(String.format("\"%s\" is not a directory.",
                Utility.getCanonicalPath(outputDirectory)));
        }

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

    public String getConfigurationOption(String name)
    {
        return options.get(name);
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

    public void buildProject(Generator generator, Map<String, String> options)
    {
        try {
            generator.generate(this);
            database.commit();
        } catch (Throwable t) {
            database.rollbackSafe();
            throw t;
        }
    }
}
