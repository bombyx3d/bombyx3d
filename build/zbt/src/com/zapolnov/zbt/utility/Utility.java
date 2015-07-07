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
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URISyntaxException;
import java.nio.charset.Charset;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.LinkOption;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;

public final class Utility
{
    public static final Charset UTF8_CHARSET = Charset.forName("UTF-8");
    public static final char[] HEX_CHARACTERS = "0123456789abcdef".toCharArray();

    public static final String OS_NAME = System.getProperty("os.name");
    public static final boolean IS_LINUX = startsWith(OS_NAME, "Linux") || startsWith(OS_NAME, "LINUX");
    public static final boolean IS_OSX = startsWith(OS_NAME, "Mac OS");
    public static final boolean IS_WINDOWS = startsWith(OS_NAME, "Windows");

    private Utility() {}

    public static byte[] md5ForString(String string)
    {
        try {
            return MessageDigest.getInstance("MD5").digest(string.getBytes(UTF8_CHARSET));
        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException(e);
        }
    }

    public static byte[] makeOptionsHash(Object... parameters)
    {
        StringBuilder builder = new StringBuilder();
        for (Object parameter : parameters) {
            if (parameter == null)
                builder.append('\2');
            else {
                builder.append('\3');
                if (parameter instanceof File)
                    builder.append(Utility.getCanonicalPath((File)parameter));
                else
                    builder.append(parameter.toString());
            }
            builder.append('\1');
        }
        return md5ForString(builder.toString());
    }

    public static boolean startsWith(String string, String prefix)
    {
        return !(string == null || prefix == null) && string.startsWith(prefix);
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

    public static boolean fileHasExtension(File file, String extension)
    {
        String fileName = file.getName();
        return (fileName.endsWith(extension) && fileName.length() > extension.length());
    }

    public static boolean fileHasExtension(File file, String[] extensions)
    {
        String fileName = file.getName();
        for (String extension : extensions) {
            if (fileName.endsWith(extension) && fileName.length() > extension.length())
                return true;
        }
        return false;
    }

    public static String getRelativePath(File fromFile, File toFile)
    {
        // This method is based on code from FileUtils.java of Apache Ant 1.9.5
        // Original code was licensed under the Apache 2.0 license
        //
        // --------------------------------------------------------------------
        // Apache Ant
        // Copyright 1999-2015 The Apache Software Foundation
        //
        // This product includes software developed at
        // The Apache Software Foundation (http://www.apache.org/).
        // --------------------------------------------------------------------

        try {
            String fromPath = fromFile.getCanonicalPath();
            String toPath = toFile.getCanonicalPath();

            // build the path stack info to compare
            String[] fromPathStack = getPathStack(fromPath);
            String[] toPathStack = getPathStack(toPath);

            if (0 < toPathStack.length && 0 < fromPathStack.length) {
                if (!fromPathStack[0].equals(toPathStack[0])) {
                    // not the same device (would be "" on Linux/Unix)
                    return getPath(Arrays.asList(toPathStack));
                }
            } else {
                // no comparison possible
                return getPath(Arrays.asList(toPathStack));
            }

            int minLength = Math.min(fromPathStack.length, toPathStack.length);
            int same = 1; // Used outside the for loop

            // get index of parts which are equal
            while (same < minLength && fromPathStack[same].equals(toPathStack[same]))
                ++same;

            List<String> relativePathStack = new ArrayList<>();

            // if "from" part is longer, fill it up with ".."
            // to reach path which is equal to both paths
            for (int i = same; i < fromPathStack.length; i++)
                relativePathStack.add("..");

            // fill it up path with parts which were not equal
            relativePathStack.addAll(Arrays.asList(toPathStack).subList(same, toPathStack.length));

            return getPath(relativePathStack);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static boolean isFileInsideDirectory(File file, File directory)
    {
        // This method is based on code from FileUtils.java of Apache Ant 1.9.5
        // Original code was licensed under the Apache 2.0 license
        //
        // --------------------------------------------------------------------
        // Apache Ant
        // Copyright 1999-2015 The Apache Software Foundation
        //
        // This product includes software developed at
        // The Apache Software Foundation (http://www.apache.org/).
        // --------------------------------------------------------------------

        try {
            String[] directoryPathStack = getPathStack(directory.getCanonicalPath());
            String[] filePathStack = getPathStack(file.getCanonicalPath());

            if (directoryPathStack.length == 0 || filePathStack.length == 0)
                return false;
            if (!directoryPathStack[0].equals(filePathStack[0]))
                return false;

            int minLength = Math.min(directoryPathStack.length, filePathStack.length);
            int same = 1;
            while (same < minLength && directoryPathStack[same].equals(filePathStack[same]))
                ++same;

            return same == directoryPathStack.length;
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static String getPath(List<String> pathStack)
    {
        // This method is based on code from FileUtils.java of Apache Ant 1.9.5
        // Original code was licensed under the Apache 2.0 license
        //
        // --------------------------------------------------------------------
        // Apache Ant
        // Copyright 1999-2015 The Apache Software Foundation
        //
        // This product includes software developed at
        // The Apache Software Foundation (http://www.apache.org/).
        // --------------------------------------------------------------------

        return getPath(pathStack, '/');
    }

    public static String getPath(List<String> pathStack, char separatorChar)
    {
        // This method is based on code from FileUtils.java of Apache Ant 1.9.5
        // Original code was licensed under the Apache 2.0 license
        //
        // --------------------------------------------------------------------
        // Apache Ant
        // Copyright 1999-2015 The Apache Software Foundation
        //
        // This product includes software developed at
        // The Apache Software Foundation (http://www.apache.org/).
        // --------------------------------------------------------------------

        final StringBuilder buffer = new StringBuilder();

        final Iterator iter = pathStack.iterator();
        if (iter.hasNext())
            buffer.append(iter.next());

        while (iter.hasNext()) {
            buffer.append(separatorChar);
            buffer.append(iter.next());
        }

        return buffer.toString();
    }

    public static String[] getPathStack(String path)
    {
        // This method is based on code from FileUtils.java of Apache Ant 1.9.5
        // Original code was licensed under the Apache 2.0 license
        //
        // --------------------------------------------------------------------
        // Apache Ant
        // Copyright 1999-2015 The Apache Software Foundation
        //
        // This product includes software developed at
        // The Apache Software Foundation (http://www.apache.org/).
        // --------------------------------------------------------------------

        String normalizedPath = path.replace(File.separatorChar, '/');
        return normalizedPath.split("/");
    }

    public static String resolveExecutable(String program)
    {
        String[] extensions = null;
        if (!Utility.IS_WINDOWS) {
            extensions = new String[]{ "" };
        } else {
            String pathext = System.getenv("PATHEXT");
            if (pathext != null)
                extensions = pathext.split(";");
            if (extensions == null || extensions.length == 0)
                extensions = new String[]{ ".com", ".exe", ".bat", ".cmd" };
        }

        String path = System.getenv("PATH");
        if (path != null) {
            String[] paths = path.split(File.pathSeparator);
            for (String directory : paths) {
                for (String extension : extensions) {
                    File file = new File(new File(directory), program + extension);
                    if (file.exists())
                        return getCanonicalPath(file);
                }
            }
        }

        return null;
    }

    public static void ensureDirectoryExists(File directory)
    {
        if (!directory.exists()) {
            if (!directory.mkdirs()) {
                throw new RuntimeException(String.format("Unable to create directory \"%s\".",
                    Utility.getCanonicalPath(directory)));
            }
        }

        if (!directory.isDirectory()) {
            throw new RuntimeException(String.format("\"%s\" is not a directory.",
                Utility.getCanonicalPath(directory)));
        }
    }

    public static void makeDirectoryHidden(File directory)
    {
        if (IS_WINDOWS) {
            String path = Utility.getCanonicalPath(directory);
            try {
                Files.setAttribute(Paths.get(path), "dos:hidden", true, LinkOption.NOFOLLOW_LINKS);
            } catch (IOException e) {
                throw new RuntimeException(
                    String.format("Unable to apply 'hidden' attribute to directory \"%s\".", path), e);
            }
        }
    }

    public static byte[] byteArrayFromFile(File file)
    {
        try (FileInputStream stream = new FileInputStream(file)) {
            int fileLength = (int)file.length();
            byte[] buffer = new byte[fileLength];
            int bytesRead = stream.read(buffer, 0, fileLength);
            if (bytesRead != fileLength) {
                throw new RuntimeException(String.format("Incomplete read in file \"%s\".",
                    Utility.getCanonicalPath(file)));
            }
            return buffer;
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static String stringFromInputStream(InputStream stream)
    {
        StringBuilder stringBuilder = new StringBuilder();
        try (InputStreamReader reader = new InputStreamReader(stream, UTF8_CHARSET)) {
            char[] buffer = new char[16384];
            int length;
            while ((length = reader.read(buffer)) >= 0)
                stringBuilder.append(buffer, 0, length);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        return stringBuilder.toString();
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

    public static String getJavaExecutable()
    {
        String javaHome = System.getProperty("java.home");
        if (javaHome != null) {
            File executableFile;
            if (!Utility.IS_WINDOWS)
                executableFile = new File(new File(javaHome), "bin/java");
            else
                executableFile = new File(new File(javaHome), "bin/java.exe");

            if (executableFile.exists())
                return Utility.getCanonicalPath(executableFile);
        }
        return "java";
    }

    public static File getApplicationJarFile()
    {
        try {
            File jar = new File(Utility.class.getProtectionDomain().getCodeSource().getLocation().toURI().getPath());
            return Utility.getCanonicalFile(jar);
        } catch (URISyntaxException e) {
            throw new RuntimeException("Unable to determine path to the application JAR file.", e);
        }
    }
}
