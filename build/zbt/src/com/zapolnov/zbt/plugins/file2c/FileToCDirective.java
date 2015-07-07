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
package com.zapolnov.zbt.plugins.file2c;

import com.zapolnov.zbt.project.Project;
import com.zapolnov.zbt.project.parser.directives.CustomDirective;
import com.zapolnov.zbt.utility.FileBuilder;
import com.zapolnov.zbt.utility.Utility;
import java.io.File;

public class FileToCDirective extends CustomDirective
{
    public final File input;
    public final String output;
    public final String identifier;
    public final String namespace;
    public final CompressionMethod compressionMethod;

    FileToCDirective(File input, String output, String identifier, String namespace, CompressionMethod compressionMethod)
    {
        this.input = input;
        this.output = output;
        this.identifier = identifier;
        this.namespace = namespace;
        this.compressionMethod = compressionMethod;
    }

    @Override public void run(Project project, File outputDirectory)
    {
        File headerFile = new File(outputDirectory, String.format("file2c/include/%s", output));
        File sourceFile = new File(outputDirectory, String.format("file2c/%s.cpp", output));

        if (!headerFile.exists() || !sourceFile.exists() || project.database().didInputFileChange(input)) {
            byte[] data = Utility.byteArrayFromFile(input);

            switch (compressionMethod)
            {
            case NONE:
                break;
            }

            FileBuilder headerBuilder = new FileBuilder(headerFile.getParentFile(), headerFile.getName());
            FileBuilder sourceBuilder = new FileBuilder(sourceFile.getParentFile(), sourceFile.getName());

            writeAutogeneratedHeader(headerBuilder);
            writeAutogeneratedHeader(sourceBuilder);

            headerBuilder.append("#pragma once\n");
            headerBuilder.append('\n');
            headerBuilder.append("#include <cstdint>\n");
            headerBuilder.append("#include <cstddef>\n");
            headerBuilder.append('\n');

            sourceBuilder.append(String.format("#include \"%s\"\n",
                Utility.getRelativePath(sourceFile.getParentFile(), headerFile)));
            sourceBuilder.append('\n');

            writeNamespaceBegin(headerBuilder);
            writeNamespaceBegin(sourceBuilder);

            headerBuilder.append(String.format("enum : size_t { %s_size = %d };\n", identifier, data.length));
            headerBuilder.append(String.format("extern const uint8_t %s[%d];\n", identifier, data.length));

            sourceBuilder.append(String.format("const uint8_t %s[%d] = {", identifier, data.length));
            for (int i = 0; i < data.length; i++) {
                if (i % 32 == 0)
                    sourceBuilder.append("\n    ");
                sourceBuilder.append("0x");
                sourceBuilder.appendHex(data[i]);
                sourceBuilder.append(',');
            }
            sourceBuilder.append("\n};\n");

            writeNamespaceEnd(headerBuilder);
            writeNamespaceEnd(sourceBuilder);

            headerBuilder.commit(project.database());
            sourceBuilder.commit(project.database());
        }
    }

    private static void writeAutogeneratedHeader(FileBuilder builder)
    {
        builder.append("// ------------------------------------------------------\n");
        builder.append("// THIS IS AN AUTOMATICALLY GENERATED FILE. DO NOT EDIT!\n");
        builder.append("// ------------------------------------------------------\n");
        builder.append('\n');
    }

    private void writeNamespaceBegin(FileBuilder builder)
    {
        if (namespace != null) {
            builder.append(String.format("namespace %s {\n", namespace));
            builder.append('\n');
        }
    }

    private void writeNamespaceEnd(FileBuilder builder)
    {
        if (namespace != null) {
            builder.append('\n');
            builder.append(String.format("} // namespace %s\n", namespace));
        }
    }
}
