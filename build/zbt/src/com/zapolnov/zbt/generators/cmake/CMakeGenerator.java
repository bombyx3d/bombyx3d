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
import com.zapolnov.zbt.project.parser.directives.CMakeUseQt5Directive;
import com.zapolnov.zbt.project.parser.directives.TargetNameDirective;
import com.zapolnov.zbt.utility.FileBuilder;
import com.zapolnov.zbt.utility.Template;
import com.zapolnov.zbt.utility.Utility;
import java.io.File;
import java.util.HashMap;
import java.util.Map;

public class CMakeGenerator extends Generator
{
    public static final String ID = "cmake3.2";
    public static final String NAME = "CMake 3.2+";

    private String targetName;
    private File outputDirectory;
    private Project project;
    private boolean useQt5;
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
            useQt5 = false;

            outputDirectory = new File(project.outputDirectory(), ID);
            Utility.ensureDirectoryExists(outputDirectory);

            project.directives().visitDirectives(new ProjectDirectiveVisitor(project, this) {
                @Override protected void visitDefine(String name, String value) {
                    //System.out.println(String.format("%s=%s", name, value));
                }
                @Override protected void visitSourceFile(File file) {
                    //System.out.println(String.format("%s", Utility.getCanonicalPath(file)));
                }
                @Override public void visitTargetName(TargetNameDirective directive) {
                    targetName = directive.name();
                }
                @Override public void visitCMakeUseQt5Directive(CMakeUseQt5Directive directive) {
                    useQt5 = directive.value();
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
        FileBuilder builder = new FileBuilder(outputDirectory, "CMakeLists.txt");

        builder.append('\n');
        builder.append("# ------------------------------------------------------\n");
        builder.append("# THIS IS AN AUTOMATICALLY GENERATED FILE. DO NOT EDIT!\n");
        builder.append("# ------------------------------------------------------\n");
        builder.append('\n');

        Map<String, String> options = new HashMap<>();
        options.put("target_name", targetName);
        options.put("use_qt5", useQt5 ? "YES" : "NO");
        template.emit(builder, options);

        builder.commit(project.database());
    }
}
