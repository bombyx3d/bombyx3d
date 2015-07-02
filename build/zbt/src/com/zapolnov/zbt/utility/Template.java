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
package com.zapolnov.zbt.utility;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;

public class Template
{
    private interface Command
    {
        void emit(FileBuilder builder);
    }

    private final static class Text implements Command
    {
        private final String text;

        public Text(String text)
        {
            this.text = text;
        }

        @Override public void emit(FileBuilder builder)
        {
            builder.append(text);
        }
    }

    private final class Variable implements Command
    {
        private final String name;

        public Variable(String name)
        {
            this.name = name;
        }

        @Override public void emit(FileBuilder builder)
        {
            String value = variables.get(name);
            if (value == null)
                throw new RuntimeException(String.format("Use of undeclared variable \"%s\" in template.", name));
            builder.append(value);
        }
    }

    private final List<Command> commands = new ArrayList<Command>();
    private Map<String, String> variables;

    public Template(InputStream stream)
    {
        String text = Utility.stringFromInputStream(stream);

        int offset = 0;
        for (;;) {
            int beginOffset = text.indexOf("@{", offset);
            if (beginOffset < 0)
                break;

            int endOffset = text.indexOf('}', beginOffset + 1);
            if (endOffset < 0)
                break;

            if (beginOffset > offset)
                commands.add(new Text(text.substring(offset, beginOffset)));

            String name = text.substring(beginOffset + 2, endOffset);
            commands.add(new Variable(name));

            offset = endOffset + 1;
        }

        if (text.length() > offset)
            commands.add(new Text(text.substring(offset, text.length())));
    }

    public void emit(FileBuilder fileBuilder, Map<String, String> variables)
    {
        try {
            this.variables = Collections.unmodifiableMap(variables);
            for (Command command : commands)
                command.emit(fileBuilder);
        } finally {
            this.variables = null;
        }
    }
}
