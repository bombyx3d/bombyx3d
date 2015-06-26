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

import com.zapolnov.zbt.generators.Generator;
import com.zapolnov.zbt.project.Project;
import com.zapolnov.zbt.project.ProjectFileReader;
import com.zapolnov.zbt.utility.Database;
import com.zapolnov.zbt.utility.Utility;
import java.io.File;
import java.util.HashMap;
import java.util.Map;

public class ProjectBuilder
{
    public static String BUILD_DIRECTORY_NAME = ".build";

    private final File projectDirectory;
    private final File projectFile;
    private final File outputDirectory;
    private final Database database;
    private final Map<String, String> options = new HashMap<>();
    private Project project;

    public ProjectBuilder(File projectDirectory)
    {
        this.projectDirectory = projectDirectory;
        this.projectFile = new File(projectDirectory, ProjectFileReader.PROJECT_FILE_NAME);
        this.outputDirectory = new File(projectDirectory, BUILD_DIRECTORY_NAME);

        createOutputDirectory();
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

    public String getOption(String name)
    {
        return options.get(name);
    }

    public void setOption(String name, String value)
    {
        options.put(name, value);
    }

    private void createOutputDirectory()
    {
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
    }

    public Project readProjectFile()
    {
        if (project == null) {
            project = new Project(projectDirectory);
            ProjectFileReader file = new ProjectFileReader(project);
            file.readFile(projectFile);
        }
        return project;
    }

    public void buildProject(Generator generator)
    {
        try {
            generator.generate(outputDirectory, project);
        } catch (Throwable t) {
            database.rollbackSafe();
            throw t;
        }

        database.commit();
    }
}
