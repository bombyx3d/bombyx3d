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

import java.io.File;
import java.io.FileReader;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import org.yaml.snakeyaml.Yaml;
import org.yaml.snakeyaml.constructor.Constructor;
import org.yaml.snakeyaml.nodes.Node;
import org.yaml.snakeyaml.reader.StreamReader;

public final class YamlParser
{
    public final static class Error extends RuntimeException
    {
        public Error(Option option, String message)
        {
            super(makeMessage(option, message, null));
        }

        public Error(Option option, Throwable t)
        {
            super(makeMessage(option, null, t));
        }

        private static String makeMessage(Option option, String message, Throwable t)
        {
            if (message == null && t != null)
                message = Utility.getExceptionMessage(t);

            if (option == null) {
                if (t instanceof RuntimeException)
                    throw (RuntimeException)t;
                throw new RuntimeException(message, t);
            }

            return String.format("Problem%s %s", option.node.getStartMark().toString(), message);
        }
    }

    public final static class Option
    {
        public final Node node;
        public final Object value;

        public Option(Node node, Object value)
        {
            this.node = node;
            this.value = value;
        }

        public boolean isSequence()
        {
            return this.value != null && this.value instanceof List;
        }

        public boolean isMapping()
        {
            return this.value != null && this.value instanceof Map;
        }

        @SuppressWarnings("unchecked") public List<Option> toSequence()
        {
            if (!isSequence())
                throw new Error(this, "Value is not a sequence.");
            return (List<Option>)this.value;
        }

        @SuppressWarnings("unchecked") public Map<Option, Option> toMapping()
        {
            if (!isMapping())
                throw new Error(this, "Value is not a mapping.");
            return (Map<Option, Option>)this.value;
        }

        @Override public String toString()
        {
            if (value == null || isSequence() || isMapping())
                return null;
            return value.toString();
        }
    }

    private YamlParser() {}

    public static Option readFile(File file)
    {
        Option root;

        try (FileReader fileReader = new FileReader(file)) {
            // Construct the YAML parser
            Yaml yaml = new Yaml(new Constructor() {
                @Override protected List<Object> createDefaultList(int size) {
                    return new ArrayList<>(size);
                }
                @Override protected Map<Object, Object> createDefaultMap() {
                    return new LinkedHashMap<>();
                }
                @Override protected Object constructObject(Node node) {
                    return new Option(node, super.constructObject(node));
                }
            });

            // Create stream reader for the file
            StreamReader reader = new StreamReader(fileReader);
            Field field = reader.getClass().getDeclaredField("name");
            field.setAccessible(true);
            field.set(reader, Utility.getCanonicalPath(file));

            // Read the file
            try {
                Method method = yaml.getClass().getDeclaredMethod("loadFromReader", StreamReader.class, Class.class);
                method.setAccessible(true);
                root = (Option)method.invoke(yaml, reader, Object.class);
            } catch (InvocationTargetException e) {
                Throwable cause = e.getCause();
                if (cause == null)
                    throw e;
                throw cause;
            }
        } catch (Throwable t) {
            String fileName = Utility.getCanonicalPath(file);
            String msg = Utility.getExceptionMessage(t);
            throw new RuntimeException(String.format("Unable to parse YAML file \"%s\".\nError: %s", fileName, msg), t);
        }

        return root;
    }
}
