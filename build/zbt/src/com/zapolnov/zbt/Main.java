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
import com.zapolnov.zbt.gui.ProjectConfigurationGui;
import com.zapolnov.zbt.project.Project;
import com.zapolnov.zbt.project.ProjectFileReader;
import com.zapolnov.zbt.utility.Database;
import com.zapolnov.zbt.utility.Utility;
import java.io.File;

public class Main
{
    public static String BUILD_DIRECTORY_NAME = ".build";

    public static void generateProject(Project project, boolean openAfterGenerate)
    {
        File projectDirectory = project.projectDirectory();
        File outputDirectory = new File(projectDirectory, BUILD_DIRECTORY_NAME);

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

        Database database = new Database(outputDirectory);
        try {
            try {
//                Generator generator = Generator.generatorForCurrentPlatform();
//                generator.generate(outputDirectory, project);
            } catch (Throwable t) {
                database.rollbackSafe();
                throw t;
            }
            database.commit();
        } finally {
            database.close();
        }
    }

    public static void main(String[] args)
    {
        boolean verbose = false;

        try {
            File projectDirectory = new File("../..");
            File projectFile = new File(projectDirectory, ProjectFileReader.PROJECT_FILE_NAME);

            Project project = new Project(projectDirectory);
            ProjectFileReader file = new ProjectFileReader(project);
            file.readFile(new File("../../project.yml"));

            ProjectConfigurationGui.run(project);
        } catch (Throwable t) {
            if (verbose)
                throw t;
            System.out.println(String.format("Error: %s", Utility.getExceptionMessage(t)));
        }
    }
}
