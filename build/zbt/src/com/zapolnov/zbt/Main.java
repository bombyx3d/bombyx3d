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
import com.zapolnov.zbt.gui.ProjectConfigurationGui;
import com.zapolnov.zbt.utility.Utility;
import java.io.File;
import java.util.HashMap;
import java.util.Map;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;

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
        System.out.println("  -t, --target              Specify target platform/compiler.");
        System.out.println("");

        System.out.println("The following combinations of target platform/compiler are allowed:");
        Map<String, Map<String, Generator.Factory>> generators = Generator.allGenerators();
        for (Map.Entry<String, Map<String, Generator.Factory>> it : generators.entrySet()) {
            for (String compiler : it.getValue().keySet()) {
                System.out.println(String.format("  \"%s / %s\"", it.getKey(), compiler));
            }
        }

        System.out.println("");
    }

    private static void handleFatalException(final Throwable t)
    {
        if (!batch || verbose)
            t.printStackTrace(System.err);
        else
            System.err.println(String.format("Error: %s", Utility.getExceptionMessage(t)));

        if (batch)
            System.exit(1);
        else {
            final String message = String.format("%s: %s", t.getClass().getName(), t.getMessage());
            SwingUtilities.invokeLater(() -> {
                JOptionPane.showMessageDialog(null, message, "Fatal error", JOptionPane.ERROR_MESSAGE);
                System.exit(1);
            });
        }
    }

    public static void main(String[] args)
    {
        Map<String, String> options = new HashMap<>();
        String target = null;

        // Parse command-line arguments

        try {

            for (int i = 0; i < args.length; i++) {
                if ("--verbose".equals(args[i]) || "-v".equals(args[i]))
                    verbose = true;
                else if ("--batch".equals(args[i]) || "-b".equals(args[i]))
                    batch = true;
                else if ("--target".equals(args[i]) || "-t".equals(args[i])) {
                    if (i == args.length - 1)
                        throw new RuntimeException(String.format("Error: missing value after the \"%s\" option.", args[i]));
                    target = args[++i];
                } else if ("--help".equals(args[i]) || "-h".equals(args[i])) {
                    showUsage();
                    System.exit(1);
                } else if (args[i] != null && !args[i].startsWith("-") && args[i].contains("=")) {
                    int index = args[i].indexOf('=');
                    String name = args[i].substring(0, index);
                    String value = args[i].substring(index + 1);
                    options.put(name, value);
                } else
                    throw new RuntimeException(String.format("Error: invalid option \"%s\".", args[i]));
            }

            // Validate the provided target platform/compiler pair

            Generator.Factory generatorFactory = null;
            String platform = null;
            String compiler = null;
            if (target != null) {
                int index = target.indexOf(" / ");
                if (index < 0) {
                    throw new RuntimeException(
                        String.format("Invalid target platform/compiler combination: \"%s\".", target));
                }

                platform = target.substring(0, index);
                compiler = target.substring(index + 3);

                Map<String, Map<String, Generator.Factory>> generators = Generator.allGenerators();
                for (Map.Entry<String, Map<String, Generator.Factory>> it : generators.entrySet()) {
                    if (it.getKey().equals(platform)) {
                        for (String jt : it.getValue().keySet()) {
                            if (jt.equals(compiler)) {
                                generatorFactory = it.getValue().get(jt);
                                break;
                            }
                        }
                        break;
                    }
                }

                if (generatorFactory == null) {
                    throw new RuntimeException(
                        String.format("Invalid target platform/compiler combination: \"%s\".", target));
                }
            }

            // Display help screen if needed

            if (!batch)
                showUsage();

            // Build the project

            final ProjectBuilder builder = new ProjectBuilder(new File("../.."));
            try {
                // Read project file

                builder.readProjectFile();

                // Apply options from the command line

                for (Map.Entry<String, String> option : options.entrySet())
                    builder.setOption(option.getKey(), option.getValue());

                // Invoke the builder

                if (batch) {
                    if (generatorFactory == null)
                        throw new RuntimeException("No target was specified on the command line.");
                    builder.buildProject(generatorFactory.create());
                } else {
                    final String platformID = platform;
                    final String compilerID = compiler;
                    SwingUtilities.invokeLater(() -> {
                        try {
                            ProjectConfigurationGui.run(builder, platformID, compilerID);
                        } catch (Throwable t) {
                            handleFatalException(t);
                        }
                    });
                }
            } finally {
                if (batch)
                    builder.closeDatabase();
            }
        } catch (Throwable t) {
            handleFatalException(t);
        }
    }
}
