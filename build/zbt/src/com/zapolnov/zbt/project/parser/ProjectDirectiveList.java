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

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class ProjectDirectiveList
{
    public final ProjectDirectiveList parent;
    private final boolean transparent;
    private final List<ProjectDirective> directives = new ArrayList<>();
    private final Set<String> enumerationIDs = new HashSet<>();

    public ProjectDirectiveList(ProjectDirectiveList parent, boolean transparent)
    {
        this.parent = parent;
        this.transparent = transparent;
    }

    public boolean reserveEnumerationID(String id)
    {
        for (ProjectDirectiveList list = this; list != null; list = list.parent) {
            if (list.enumerationIDs.contains(id))
                return false;
        }

        for (ProjectDirectiveList list = this; list != null; list = list.parent) {
            list.enumerationIDs.add(id);
            if (!list.transparent)
                break;
        }

        return true;
    }

    public void addDirective(ProjectDirective directive)
    {
        directives.add(directive);
    }

    public void visitDirectives(ProjectDirectiveVisitor visitor)
    {
        for (ProjectDirective directive : directives)
            directive.visit(visitor);
    }
}
