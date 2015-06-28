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
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.ArrayList;
import java.util.List;

public final class Utility
{
    public static final String OS_NAME = System.getProperty("os.name");
    public static final boolean IS_LINUX = startsWith(OS_NAME, "Linux") || startsWith(OS_NAME, "LINUX");
    public static final boolean IS_OSX = startsWith(OS_NAME, "Mac OS");
    public static final boolean IS_WINDOWS = startsWith(OS_NAME, "Windows");

    private Utility() {}

    public static boolean startsWith(String string, String prefix)
    {
        if (string == null || prefix == null)
            return false;
        return string.startsWith(prefix);
    }

    public static File getCanonicalFile(File file)
    {
        try {
            return file.getCanonicalFile();
        } catch (Throwable ignored) {
            return file.getAbsoluteFile();
        }
    }

    public static String getCanonicalPath(File file)
    {
        try {
            return file.getCanonicalPath();
        } catch (Throwable ignored) {
            return file.getAbsolutePath();
        }
    }

    public static String getExceptionMessage(Throwable throwable)
    {
        if (throwable instanceof FileNotFoundException)
            return String.format("File not found: %s", throwable.getMessage());

        String message = throwable.getMessage();
        if (message == null)
            message = String.format("Unhandled exception: %s", throwable.getClass().getName());

        return message;
    }

    public static List<File> recursivelyEnumerateFilesInDirectory(File directory)
    {
        final List<File> files = new ArrayList<>();
        System.out.println(String.format("Enumerating files in source directory \"%s\".", directory));

        try {
            final Path path = Paths.get(directory.getAbsolutePath());

            Files.walkFileTree(path, new SimpleFileVisitor<Path>() {
                @Override public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) throws IOException {
                    files.add(file.toFile().getCanonicalFile());
                    return FileVisitResult.CONTINUE;
                }
            });
        } catch (IOException e) {
            String message = String.format("Unable to enumerate files in directory \"%s\".", directory.getAbsolutePath());
            throw new RuntimeException(message, e);
        }

        return files;
    }
}
