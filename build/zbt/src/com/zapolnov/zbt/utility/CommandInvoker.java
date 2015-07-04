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

import java.io.BufferedReader;
import java.io.File;
import java.io.InputStream;
import java.io.InputStreamReader;

public final class CommandInvoker
{
    public interface Printer
    {
        void print(String message);
    }

    private final static class Future
    {
        private volatile boolean complete = false;
        public synchronized void notifyFuture() { complete = true; notifyAll(); }
        public synchronized void waitFuture() throws InterruptedException { while (!complete) wait(1000); }
    }

    private File workingDirectory;
    private Printer printer;

    public CommandInvoker(File workingDirectory, Printer printer)
    {
        this.workingDirectory = workingDirectory;
        this.printer = printer;
    }

    public static String commandToString(String... args)
    {
        StringBuilder builder = new StringBuilder();
        String prefix = "";

        for (String string : args) {
            builder.append(prefix);
            prefix = " ";

            if (!string.contains(" "))
                builder.append(string);
            else {
                builder.append('"');
                builder.append(string);
                builder.append('"');
            }
        }

        return builder.toString();
    }

    public void invoke(String... command)
    {
        int exitValue;
        try {
            System.out.println(String.format("Invoking command: %s", commandToString(command)));
            System.out.println(String.format("Working directory: %s", Utility.getCanonicalPath(workingDirectory)));

            ProcessBuilder builder = new ProcessBuilder(command);
            builder.directory(workingDirectory);
            builder.redirectErrorStream(true);
            Process process = builder.start();

            final InputStream stream = process.getInputStream();
            final Future future = new Future();

            Thread thread = new Thread(() -> {
                final BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
                try {
                    try {
                        for (;;) {
                            String line = reader.readLine();
                            if (line == null)
                                break;

                            if (printer != null)
                                printer.print(line + '\n');
                            else
                                System.out.println(line);
                       }
                    } finally {
                        stream.close();
                    }
                } catch (Throwable t) {
                    t.printStackTrace(System.err);
                }
                future.notifyFuture();
            });
            thread.setDaemon(true);
            thread.start();

            process.waitFor();
            future.waitFuture();

            exitValue = process.exitValue();
            System.out.println(String.format("Command has exited with code %d", exitValue));
        } catch (Throwable t) {
            throw new RuntimeException(String.format("Unable to invoke command [%s].", commandToString(command)), t);
        }

        if (exitValue != 0) {
            throw new RuntimeException(String.format("Command [%s] has exited with code %d",
                commandToString(command), exitValue));
        }
    }
}
