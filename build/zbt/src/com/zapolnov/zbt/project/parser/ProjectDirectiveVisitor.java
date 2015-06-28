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

import com.zapolnov.zbt.project.Project;
import com.zapolnov.zbt.project.parser.directives.DefineDirective;
import com.zapolnov.zbt.project.parser.directives.ImportDirective;
import com.zapolnov.zbt.project.parser.directives.SelectorDirective;
import com.zapolnov.zbt.project.parser.directives.SourceDirectoriesDirective;
import com.zapolnov.zbt.utility.Utility;
import java.io.File;
import java.util.HashSet;
import java.util.Set;

public abstract class ProjectDirectiveVisitor extends AbstractProjectDirectiveVisitor
{
    private final Project project;
    private final Set<String> visitedSourceFiles = new HashSet<>();
    private final Set<String> importedModule = new HashSet<>();

    public ProjectDirectiveVisitor(Project project)
    {
        this.project = project;
    }

    protected void visitDefine(String name, String value) {}
    protected void visitSourceFile(File file) {}

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
            if (visitedSourceFiles.add(canonicalPath))
                visitSourceFile(file);
        }
    }

    @Override public void visitImport(ImportDirective directive)
    {
        if (importedModule.add(directive.modulePath()))
            directive.innerDirectives().visitDirectives(this);
    }

    @Override public void visitSelector(SelectorDirective directive)
    {
        String value = project.getConfigurationOption(directive.enumerationID());
        if (value != null && directive.matchingValues().contains(value))
            directive.innerDirectives().visitDirectives(this);
    }
}
