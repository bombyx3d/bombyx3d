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
package com.zapolnov.zbt.project.directive;

import com.zapolnov.zbt.project.ProjectDirective;
import com.zapolnov.zbt.project.ProjectVisitor;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public final class SelectorDirective extends ProjectDirective
{
    private final String enumerationName;
    private final String enumerationValue;
    private final List<ProjectDirective> innerDirectives;

    public SelectorDirective(String enumerationName, String enumerationValue, List<ProjectDirective> innerDirectives)
    {
        this.enumerationName = enumerationName;
        this.enumerationValue = enumerationValue;
        this.innerDirectives = new ArrayList<>(innerDirectives);
    }

    public String enumerationName()
    {
        return enumerationName;
    }

    public String enumerationValue()
    {
        return enumerationValue;
    }

    @Override public void visit(ProjectVisitor visitor)
    {
        visitor.visitSelector(this);
    }

    public void visitDirectives(ProjectVisitor visitor)
    {
        for (ProjectDirective directive : innerDirectives)
            directive.visit(visitor);
    }
}

