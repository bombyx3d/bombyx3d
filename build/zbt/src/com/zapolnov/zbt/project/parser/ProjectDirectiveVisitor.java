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

import com.zapolnov.zbt.generators.Generator;
import com.zapolnov.zbt.project.Project;
import com.zapolnov.zbt.project.parser.directives.CustomDirectiveWrapper;
import com.zapolnov.zbt.project.parser.directives.DefineDirective;
import com.zapolnov.zbt.project.parser.directives.GeneratorSelectorDirective;
import com.zapolnov.zbt.project.parser.directives.HeaderPathsDirective;
import com.zapolnov.zbt.project.parser.directives.ImportDirective;
import com.zapolnov.zbt.project.parser.directives.RootProjectSelectorDirective;
import com.zapolnov.zbt.project.parser.directives.SelectorDirective;
import com.zapolnov.zbt.project.parser.directives.SourceDirectoriesDirective;
import com.zapolnov.zbt.project.parser.directives.ThirdPartyHeaderPathsDirective;
import com.zapolnov.zbt.project.parser.directives.ThirdPartySourceDirectoriesDirective;
import com.zapolnov.zbt.utility.Utility;
import java.io.File;
import java.util.HashSet;
import java.util.Set;

public abstract class ProjectDirectiveVisitor extends AbstractProjectDirectiveVisitor
{
    private final Project project;
    private final File outputDirectory;
    private final Generator generator;
    private final Set<String> visitedSourceFiles = new HashSet<>();
    private final Set<String> visitedHeaderPaths = new HashSet<>();
    private final Set<String> importedModule = new HashSet<>();

    public ProjectDirectiveVisitor(Project project, File outputDirectory, Generator generator)
    {
        this.project = project;
        this.outputDirectory = outputDirectory;
        this.generator = generator;
    }

    protected void visitDefine(String name, String value) {}
    protected void visitSourceFile(File file) {}
    protected void visitThirdPartySourceFile(File file) {}
    protected void visitHeaderPath(File directory) {}
    protected void visitThirdPartyHeaderPath(File directory) {}
    protected void visitProjectFile(File file) {}

    @Override public void visitDefine(DefineDirective directive)
    {
        for (String define : directive.defines()) {
            int index = define.indexOf('=');
            if (index < 0)
                visitDefine(define, null);
            else {
                String name = define.substring(0, index);
                String value = define.substring(index + 1);
                visitDefine(name, value);
            }
        }
    }

    @Override public void visitSourceDirectories(SourceDirectoriesDirective directive)
    {
        for (File file : directive.sourceFiles()) {
            String canonicalPath = Utility.getCanonicalPath(file);
            if (visitedSourceFiles.add(canonicalPath)) {
                if (!Utility.isFileInsideDirectory(file, project.outputDirectory()))
                    visitSourceFile(file);
            }
        }
    }

    @Override public void visitThirdPartySourceDirectories(ThirdPartySourceDirectoriesDirective directive)
    {
        for (File file : directive.sourceFiles()) {
            String canonicalPath = Utility.getCanonicalPath(file);
            if (visitedSourceFiles.add(canonicalPath)) {
                if (!Utility.isFileInsideDirectory(file, project.outputDirectory()))
                    visitThirdPartySourceFile(file);
            }
        }
    }

    @Override public void visitHeaderPaths(HeaderPathsDirective directive)
    {
        for (File file : directive.headerPaths()) {
            String canonicalPath = Utility.getCanonicalPath(file);
            if (visitedHeaderPaths.add(canonicalPath))
                visitHeaderPath(file);
        }
    }

    @Override public void visitThirdPartyHeaderPaths(ThirdPartyHeaderPathsDirective directive)
    {
        for (File file : directive.headerPaths()) {
            String canonicalPath = Utility.getCanonicalPath(file);
            if (visitedHeaderPaths.add(canonicalPath))
                visitThirdPartyHeaderPath(file);
        }
    }

    @Override public void visitImport(ImportDirective directive)
    {
        if (importedModule.add(directive.modulePath())) {
            visitProjectFile(new File(directive.modulePath()));
            directive.innerDirectives().visitDirectives(this);
        }
    }

    @Override public void visitSelector(SelectorDirective directive)
    {
        String value = project.getConfigurationOption(directive.enumerationID());
        if (value != null && directive.matchingValues().contains(value))
            directive.innerDirectives().visitDirectives(this);
    }

    @Override public void visitGeneratorSelector(GeneratorSelectorDirective directive)
    {
        String value = generator.id();

        ProjectDirectiveList list = directive.mapping().get(value);
        if (list == null)
            list = directive.mapping().get(GeneratorSelectorDirective.DEFAULT);

        if (list != null)
            list.visitDirectives(this);
    }

    @Override public void visitRootProjectSelector(RootProjectSelectorDirective directive)
    {
        if (directive.isTrue)
            directive.innerDirectives().visitDirectives(this);
    }

    @Override public void visitCustomDirectiveWrapper(CustomDirectiveWrapper directive)
    {
        directive.directive.run(project, outputDirectory);
    }
}
