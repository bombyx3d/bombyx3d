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
package com.zapolnov.zbt.generators.cmake;

import com.zapolnov.zbt.generators.Generator;
import com.zapolnov.zbt.project.Project;
import com.zapolnov.zbt.project.parser.ProjectDirectiveVisitor;
import com.zapolnov.zbt.project.parser.directives.CMakeUseOpenGLDirective;
import com.zapolnov.zbt.project.parser.directives.CMakeUseQt5Directive;
import com.zapolnov.zbt.project.parser.directives.TargetNameDirective;
import com.zapolnov.zbt.utility.FileBuilder;
import com.zapolnov.zbt.utility.Template;
import com.zapolnov.zbt.utility.Utility;
import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class CMakeGenerator extends Generator
{
    public static final String ID = "cmake3.2";
    public static final String NAME = "CMake 3.2+";

    private String targetName;
    private File outputDirectory;
    private Project project;
    private Map<String, String> defines;
    private List<File> sourceFiles;
    private boolean useQt5;
    private boolean useOpenGL;
    private final Template template;

    public CMakeGenerator()
    {
        template = new Template(getClass().getResourceAsStream("CMakeLists.template"));
    }

    @Override public String id()
    {
        return ID;
    }

    @Override public String name()
    {
        return NAME;
    }

    @Override public void generate(final Project project)
    {
        try {
            this.project = project;
            targetName = "App";
            defines = new HashMap<>();
            sourceFiles = new ArrayList<>();
            useQt5 = false;
            useOpenGL = false;

            outputDirectory = new File(project.outputDirectory(), ID);
            Utility.ensureDirectoryExists(outputDirectory);

            project.directives().visitDirectives(new ProjectDirectiveVisitor(project, this) {
                @Override protected void visitDefine(String name, String value) {
                    defines.put(name, value != null ? value : "1");
                }
                @Override protected void visitSourceFile(File file) {
                    sourceFiles.add(Utility.getCanonicalFile(file));
                }
                @Override public void visitTargetName(TargetNameDirective directive) {
                    targetName = directive.name();
                }
                @Override public void visitCMakeUseQt5Directive(CMakeUseQt5Directive directive) {
                    useQt5 = directive.value();
                }
                @Override public void visitCMakeUseOpenGLDirective(CMakeUseOpenGLDirective directive) {
                    useOpenGL = directive.value();
                }
            });

            writeCMakeLists();
        } finally {
            targetName = null;
            outputDirectory = null;
            this.project = null;
        }
    }

    private void writeCMakeLists()
    {
        StringBuilder definitions = new StringBuilder();
        if (!defines.isEmpty()) {
            definitions.append("add_definitions(");
            for (Map.Entry<String, String> define : defines.entrySet()) {
                String value = define.getValue().replace("\\", "\\\\").replace("\"", "\\\"");
                definitions.append(String.format("\n    \"-D%s=%s\"", define.getKey(), value));
            }
            definitions.append(")");
        }

        List<String> sourcePaths = new ArrayList<>();
        Map<String, List<String>> sourceGroups = new HashMap<>();
        for (File source : sourceFiles) {
            String path = Utility.getRelativePath(outputDirectory, source);
            sourcePaths.add(path);

            File file = Utility.getCanonicalFile(source);
            String sourceGroup = Utility.getRelativePath(project.projectDirectory(), file.getParentFile());
            List<String> list = sourceGroups.get(sourceGroup);
            if (list == null) {
                list = new ArrayList<>();
                sourceGroups.put(sourceGroup, list);
            }
            list.add(path);
        }

        // Write CMakeLists.txt

        FileBuilder builder = new FileBuilder(outputDirectory, "CMakeLists.txt");
        writeAutoGeneratedHeader(builder);

        Map<String, String> options = new HashMap<>();
        options.put("target_name", targetName);
        options.put("use_qt5", useQt5 ? "YES" : "NO");
        options.put("use_opengl", useOpenGL ? "YES" : "NO");
        options.put("defines", definitions.toString());
        template.emit(builder, options);

        builder.commit(project.database());

        // Write SourceFiles.cmake

        builder = new FileBuilder(outputDirectory, "SourceFiles.cmake");
        writeAutoGeneratedHeader(builder);

        builder.append("set(source_files");
        for (String path : sourcePaths) {
            builder.append(String.format("\n    \"%s\"", path));
        }
        builder.append(")\n");

        builder.commit(project.database());

        // Write SourceGroups.cmake

        builder = new FileBuilder(outputDirectory, "SourceGroups.cmake");
        writeAutoGeneratedHeader(builder);

        for (Map.Entry<String, List<String>> sourceGroup : sourceGroups.entrySet()) {
            String groupName = sourceGroup.getKey().replace("/", "\\\\");
            builder.append(String.format("source_group(\"%s\" FILES", groupName));
            for (String file : sourceGroup.getValue())
                builder.append(String.format("\n    \"%s\"", file));
            builder.append(")\n\n");
        }

        builder.commit(project.database());
    }

    private void writeAutoGeneratedHeader(FileBuilder builder)
    {
        builder.append('\n');
        builder.append("# ------------------------------------------------------\n");
        builder.append("# THIS IS AN AUTOMATICALLY GENERATED FILE. DO NOT EDIT!\n");
        builder.append("# ------------------------------------------------------\n");
        builder.append('\n');
    }
}
