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
import java.io.FileOutputStream;
import java.io.IOException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public final class FileBuilder
{
    private final StringBuilder stringBuilder;
    private final File file;

    public FileBuilder(File directory, String fileName)
    {
        this(new File(directory, fileName));
    }

    public FileBuilder(File file)
    {
        this.file = Utility.getCanonicalFile(file);
        this.stringBuilder = new StringBuilder();
        Utility.ensureDirectoryExists(this.file.getParentFile());
    }

    public void append(char ch)
    {
        stringBuilder.append(ch);
    }

    public void append(String string)
    {
        stringBuilder.append(string);
    }

    public void appendHex(byte value)
    {
        stringBuilder.append(Utility.HEX_CHARACTERS[(value >> 4) & 0xF]);
        stringBuilder.append(Utility.HEX_CHARACTERS[value & 0xF]);
    }

    public void commit(Database database)
    {
        try {
            String text = stringBuilder.toString();
            byte[] bytes = text.getBytes(Utility.UTF8_CHARSET);

            if (file.exists()) {
                MessageDigest md = MessageDigest.getInstance("MD5");
                byte[] md5 = md.digest(bytes);
                if (!database.didOutputFileChange(file, md5))
                    return;
            }

            System.out.println(String.format("Writing %s", Utility.getRelativePath(database.directory, file)));
            try (FileOutputStream stream = new FileOutputStream(file)) {
                stream.write(bytes);
                stream.flush();
            }
        } catch (NoSuchAlgorithmException|IOException e) {
            throw new RuntimeException(e);
        }
    }
}
