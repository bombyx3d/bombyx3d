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
package com.zapolnov.zbt;

import com.zapolnov.zbt.generators.Generator;
import com.zapolnov.zbt.gui.FatalErrorDialog;
import com.zapolnov.zbt.gui.MainDialog;
import com.zapolnov.zbt.project.Project;
import com.zapolnov.zbt.utility.Utility;
import java.io.File;
import java.util.LinkedHashMap;
import java.util.Map;

public class Main
{
    private static boolean verbose = false;
    private static boolean batch = false;

    private static void showUsage()
    {
                         // 12345678901234567890123456789012345678901234567890123456789012345678901234567890
        System.out.println("");
        System.out.println("Usage: zbt [options] [name=value]...");
        System.out.println("Where options are:");
        System.out.println("  -h, --help                Display this help screen.");
        System.out.println("  -v, --verbose             Verbose error reporting.");
        System.out.println("  -b, --batch               Batch mode (no GUI).");
        System.out.println("  -g, --generator           Specify generator to use.");
        System.out.println("  -p, --project             Path to the source directory of the project.");
        System.out.println("");

        System.out.println("The following generators are available:");
        Map<String, Generator> generators = Generator.allGenerators();
        for (Map.Entry<String, Generator> it : generators.entrySet()) {
            System.out.println(String.format("  \"%s\"", it.getKey()));
        }

        System.out.println("");
    }

    private static void handleFatalException(final Throwable t)
    {
        if (batch) {
            if (verbose)
                t.printStackTrace();
            else
                System.err.println(String.format("Error: %s", Utility.getExceptionMessage(t)));
            System.exit(1);
        }

        FatalErrorDialog.run(t);
    }

    public static void main(String[] args)
    {
        try {
            Map<String, String> options = new LinkedHashMap<>();
            File projectPath = Utility.getCanonicalFile(new File("."));
            Generator generator = null;

            // Parse command-line arguments

            for (int i = 0; i < args.length; i++) {
                if ("--verbose".equals(args[i]) || "-v".equals(args[i]))
                    verbose = true;
                else if ("--batch".equals(args[i]) || "-b".equals(args[i]))
                    batch = true;
                else if ("--generator".equals(args[i]) || "-g".equals(args[i])) {
                    if (i == args.length - 1)
                        throw new RuntimeException(String.format("Missing value after the \"%s\" option.", args[i]));
                    String generatorName = args[++i];
                    generator = Generator.allGenerators().get(generatorName);
                    if (generator == null)
                        throw new RuntimeException(String.format("Invalid generator \"%s\".", generatorName));
                } else if ("--project".equals(args[i]) || "-p".equals(args[i])) {
                    if (i == args.length - 1)
                        throw new RuntimeException(String.format("Missing value after the \"%s\" option.", args[i]));
                    projectPath = Utility.getCanonicalFile(new File(args[++i]));
                    if (!projectPath.exists())
                        throw new RuntimeException(String.format("Directory does not exist: \"%s\".", projectPath));
                    if (!projectPath.isDirectory())
                        throw new RuntimeException(String.format("\"%s\" is not a directory.", projectPath));
                } else if ("--help".equals(args[i]) || "-h".equals(args[i])) {
                    showUsage();
                    System.exit(1);
                } else if (args[i] != null && !args[i].startsWith("-") && args[i].contains("=")) {
                    int index = args[i].indexOf('=');
                    String name = args[i].substring(0, index);
                    String value = args[i].substring(index + 1);
                    options.put(name, value);
                } else
                    throw new RuntimeException(String.format("Invalid option \"%s\".", args[i]));
            }

            // Display help screen if needed

            if (!batch)
                showUsage();

            // Build the project

            final Project project = new Project(projectPath);

            // Invoke the builder

            try {
                if (!batch) {
                    try {
                        MainDialog.run(project, generator, options);
                    } finally {
                        project.closeDatabase();
                    }
                } else {
                    if (generator == null)
                        throw new RuntimeException("No generator was specified on the command line.");
                    project.build(generator, options, null, error -> {
                        project.closeDatabase();
                        if (error != null)
                            handleFatalException(error);
                        System.exit(0);
                    });

                    for (;;)
                        Thread.sleep(1000);
                }
            } catch (Throwable t) {
                project.closeDatabase();
                throw t;
            }
        } catch (Throwable t) {
            handleFatalException(t);
        }

        System.exit(0);
    }
}
