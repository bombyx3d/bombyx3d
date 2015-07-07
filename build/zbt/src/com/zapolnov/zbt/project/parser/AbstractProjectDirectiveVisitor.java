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

import com.zapolnov.zbt.project.parser.directives.CMakeUseOpenGLDirective;
import com.zapolnov.zbt.project.parser.directives.CMakeUseQt5Directive;
import com.zapolnov.zbt.project.parser.directives.CustomDirectiveWrapper;
import com.zapolnov.zbt.project.parser.directives.DefineDirective;
import com.zapolnov.zbt.project.parser.directives.EnumerationDirective;
import com.zapolnov.zbt.project.parser.directives.HeaderPathsDirective;
import com.zapolnov.zbt.project.parser.directives.RootProjectSelectorDirective;
import com.zapolnov.zbt.project.parser.directives.TargetNameDirective;
import com.zapolnov.zbt.project.parser.directives.GeneratorSelectorDirective;
import com.zapolnov.zbt.project.parser.directives.ImportDirective;
import com.zapolnov.zbt.project.parser.directives.SelectorDirective;
import com.zapolnov.zbt.project.parser.directives.SourceDirectoriesDirective;
import com.zapolnov.zbt.project.parser.directives.ThirdPartyHeaderPathsDirective;
import com.zapolnov.zbt.project.parser.directives.ThirdPartySourceDirectoriesDirective;

public abstract class AbstractProjectDirectiveVisitor
{
    public void visitDirective(ProjectDirective directive) {}
    public void visitDefine(DefineDirective directive) {}
    public void visitSourceDirectories(SourceDirectoriesDirective directive) {}
    public void visitThirdPartySourceDirectories(ThirdPartySourceDirectoriesDirective directive) {}
    public void visitHeaderPaths(HeaderPathsDirective directive) {}
    public void visitThirdPartyHeaderPaths(ThirdPartyHeaderPathsDirective directive) {}
    public void visitEnumeration(EnumerationDirective directive) {}
    public void visitImport(ImportDirective directive) {}
    public void visitSelector(SelectorDirective directive) {}
    public void visitGeneratorSelector(GeneratorSelectorDirective directive) {}
    public void visitRootProjectSelector(RootProjectSelectorDirective directive) {}
    public void visitTargetName(TargetNameDirective directive) {}
    public void visitCMakeUseOpenGLDirective(CMakeUseOpenGLDirective directive) {}
    public void visitCMakeUseQt5Directive(CMakeUseQt5Directive directive) {}
    public void visitCustomDirectiveWrapper(CustomDirectiveWrapper directive) {}
}
