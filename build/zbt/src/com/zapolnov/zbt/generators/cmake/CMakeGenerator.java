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
package com.zapolnov.zbt.generators.cmake;

import com.zapolnov.zbt.Main;
import com.zapolnov.zbt.generators.Generator;
import com.zapolnov.zbt.project.Project;
import com.zapolnov.zbt.project.parser.ProjectDirectiveVisitor;
import com.zapolnov.zbt.project.parser.ProjectFileParser;
import com.zapolnov.zbt.project.parser.directives.CMakeUseOpenGLDirective;
import com.zapolnov.zbt.project.parser.directives.CMakeUseQt5Directive;
import com.zapolnov.zbt.project.parser.directives.TargetNameDirective;
import com.zapolnov.zbt.utility.CommandInvoker;
import com.zapolnov.zbt.utility.Database;
import com.zapolnov.zbt.utility.FileBuilder;
import com.zapolnov.zbt.utility.GuiUtility;
import com.zapolnov.zbt.utility.Template;
import com.zapolnov.zbt.utility.Utility;
import java.awt.Desktop;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import javax.swing.BoxLayout;
import javax.swing.JComboBox;
import javax.swing.JPanel;

public class CMakeGenerator extends Generator
{
    private enum BuildTool
    {
        UNIX_MAKEFILES("Unix Makefiles", "makefiles", "Unix Makefiles", true),
        XCODE("Xcode", "xcode", "Xcode", false),
        MINGW32("MinGW (32-bit)", "mingw32", cmakeMinGWGenerator(), true, "Z_MINGW_CFLAGS=-m32"),
        MINGW64("MinGW (64-bit)", "mingw64", cmakeMinGWGenerator(), true, "Z_MINGW_CFLAGS=-m64"),
        VS2013_WIN32("Visual Studio 2013 (32-bit)", "vs2013_win32", "Visual Studio 12 2013", false),
        VS2013_WIN64("Visual Studio 2013 (64-bit)", "vs2013_win64", "Visual Studio 12 2013 Win64", false),
        VS2015_WIN32("Visual Studio 2015 (32-bit)", "vs2015_win32", "Visual Studio 14 2015", false),
        VS2015_WIN64("Visual Studio 2015 (64-bit)", "vs2015_win64", "Visual Studio 14 2015 Win64", false);

        public final String title;
        public final String directoryName;
        public final String cmakeGenerator;
        public final boolean acceptsBuildType;
        public final String[] defines;

        BuildTool(String title, String directoryName, String cmakeGenerator, boolean acceptsBuildType, String... defines)
        {
            this.title = title;
            this.directoryName = directoryName;
            this.cmakeGenerator = cmakeGenerator;
            this.acceptsBuildType = acceptsBuildType;
            this.defines = defines;
        }
    }

    public static final String ID = "cmake3.2";
    public static final String NAME = "CMake 3.2+";
    public static final String SOURCE_DIRECTORY = "src";

    private String targetName;
    private File outputDirectory;
    private Project project;
    private Map<String, String> defines;
    private List<File> sourceFiles;
    private List<File> headerFiles;
    private List<File> thirdPartySourceFiles;
    private List<File> thirdPartyHeaderFiles;
    private List<File> headerPaths;
    private List<File> thirdPartyHeaderPaths;
    private List<File> projectFiles;
    private boolean useQt5;
    private boolean useOpenGL;
    private final Template rootTemplate;
    private final Template srcTemplate;
    private JComboBox<String> buildToolCombo;
    private JComboBox<String> buildTypeCombo;
    private BuildTool selectedBuildTool;
    private String selectedBuildType;
    private String qt5Path;
    private String cmakeExecutable;

    public CMakeGenerator()
    {
        rootTemplate = new Template(getClass().getResourceAsStream("root-CMakeLists.template"));
        srcTemplate = new Template(getClass().getResourceAsStream("src-CMakeLists.template"));
    }

    @Override public String id()
    {
        return ID;
    }

    @Override public String name()
    {
        return NAME;
    }

    @Override public JPanel createSettingsPanel(Database database)
    {
        JPanel panel = new JPanel();
        panel.setLayout(new BoxLayout(panel, BoxLayout.PAGE_AXIS));

        String savedBuildTool = database.getOption(Database.OPTION_CMAKE_BUILD_TOOL);
        if (savedBuildTool == null)
            savedBuildTool = buildTools().keySet().iterator().next();

        String savedBuildType = database.getOption(Database.OPTION_CMAKE_BUILD_TYPE);
        if (savedBuildType == null)
            savedBuildType = "Debug";

        buildToolCombo = GuiUtility.createComboBox(panel, "Build tool:",
            buildTools().keySet(), savedBuildTool);
        buildTypeCombo = GuiUtility.createComboBox(panel, "Build type:",
            new String[]{ "Debug", "Release", "RelWithDebInfo", "MinSizeRel" }, savedBuildType);

        buildToolCombo.addItemListener(e -> updateUI());
        buildTypeCombo.addItemListener(e -> updateUI());
        updateUI();

        return panel;
    }

    @Override public void validateAndSaveSettings(Database database)
    {
        selectedBuildTool = selectedBuildTool();
        if (selectedBuildTool == null)
            throw new RuntimeException("Build tool has not been selected.");

        if (!selectedBuildTool.acceptsBuildType) {
            selectedBuildType = null;
        } else {
            selectedBuildType = selectedBuildType();
            if (selectedBuildType == null)
                throw new RuntimeException("Build type has not been selected.");
        }

        cmakeExecutable = findCMakeExecutable();
        if (cmakeExecutable == null)
            throw new RuntimeException("CMake was not found in PATH.");

        database.setOption(Database.OPTION_CMAKE_BUILD_TOOL, (String) buildToolCombo.getSelectedItem());
        if (selectedBuildType != null)
            database.setOption(Database.OPTION_CMAKE_BUILD_TYPE, selectedBuildType);
    }

    public void setSelectedBuildTool(String buildTool, String buildType)
    {
        selectedBuildTool = buildTools().get(buildTool);
        selectedBuildType = buildType;

        cmakeExecutable = findCMakeExecutable();
        if (cmakeExecutable == null)
            throw new RuntimeException("CMake was not found in PATH.");
    }

    public void setQt5Path(String path)
    {
        qt5Path = path;
    }

    @Override public void generate(final Project project, CommandInvoker.Printer printer, boolean build)
    {
        try {
            this.project = project;
            targetName = "App";
            defines = new LinkedHashMap<>();
            sourceFiles = new ArrayList<>();
            headerFiles = new ArrayList<>();
            thirdPartySourceFiles = new ArrayList<>();
            thirdPartyHeaderFiles = new ArrayList<>();
            headerPaths = new ArrayList<>();
            thirdPartyHeaderPaths = new ArrayList<>();
            projectFiles = new ArrayList<>();
            useQt5 = false;
            useOpenGL = false;

            // Create output directory

            String outputDirectoryName;
            if (selectedBuildTool == null) {
                outputDirectoryName = String.format("%s/%s", ID, "default");
            } else {
                outputDirectoryName = String.format("%s/%s", ID, selectedBuildTool.directoryName);
                if (selectedBuildType != null)
                    outputDirectoryName = String.format("%s/%s", outputDirectoryName, selectedBuildType);
            }

            outputDirectory = new File(project.outputDirectory(), outputDirectoryName);
            Utility.ensureDirectoryExists(outputDirectory);

            // Collect information from the project file

            project.directives().visitDirectives(new ProjectDirectiveVisitor(project, outputDirectory, this) {
                @Override protected void visitDefine(String name, String value) {
                    defines.put(name, value != null ? value : "1");
                }
                @Override protected void visitSourceFile(File file) {
                    if (isSourceFile(file))
                        sourceFiles.add(Utility.getCanonicalFile(file));
                    else if (isHeaderFile(file))
                        headerFiles.add(Utility.getCanonicalFile(file));
                }
                @Override protected void visitThirdPartySourceFile(File file) {
                    if (isSourceFile(file))
                        thirdPartySourceFiles.add(Utility.getCanonicalFile(file));
                    else if (isHeaderFile(file))
                        thirdPartyHeaderFiles.add(Utility.getCanonicalFile(file));
                }
                @Override protected void visitHeaderPath(File directory) {
                    headerPaths.add(Utility.getCanonicalFile(directory));
                }
                @Override protected void visitThirdPartyHeaderPath(File directory) {
                    thirdPartyHeaderPaths.add(Utility.getCanonicalFile(directory));
                }
                @Override public void visitTargetName(TargetNameDirective directive) {
                    targetName = directive.name();
                }
                @Override public void visitProjectFile(File file) {
                    projectFiles.add(Utility.getCanonicalFile(file));
                }
                @Override public void visitCMakeUseQt5Directive(CMakeUseQt5Directive directive) {
                    useQt5 = directive.value();
                }
                @Override public void visitCMakeUseOpenGLDirective(CMakeUseOpenGLDirective directive) {
                    useOpenGL = directive.value();
                }
            });

            // Try to find Qt5 installation directory

            if (qt5Path == null && selectedBuildTool != null && useQt5)
                qt5Path = findQt5InstallationDirectory(selectedBuildTool);

            // Generate files

            writeCMakeLists(selectedBuildTool, selectedBuildType, qt5Path);

            // Run CMake

            if (cmakeExecutable != null && selectedBuildTool != null) {

                // Run CMake to generate project files

                List<String> cmakeCommand = new ArrayList<>();
                cmakeCommand.add(cmakeExecutable);
                cmakeCommand.add("-G");
                cmakeCommand.add(selectedBuildTool.cmakeGenerator);
                if (selectedBuildTool.acceptsBuildType)
                    cmakeCommand.add(String.format("-DCMAKE_BUILD_TYPE=%s", selectedBuildType));
                if (qt5Path != null)
                    cmakeCommand.add(String.format("-DCMAKE_PREFIX_PATH=%s", qt5Path));
                cmakeCommand.add("-DCMAKE_INSTALL_PREFIX=_INSTALL_");
                for (String define : selectedBuildTool.defines)
                    cmakeCommand.add(String.format("-D%s", define));
                cmakeCommand.add(".");

                CommandInvoker commandInvoker = new CommandInvoker(outputDirectory, printer);
                commandInvoker.invoke(cmakeCommand.toArray(new String[cmakeCommand.size()]));

                // Run CMake to build project

                if (build) {
                    cmakeCommand = new ArrayList<>();
                    cmakeCommand.add(cmakeExecutable);
                    cmakeCommand.add("--build");
                    cmakeCommand.add(".");

                    commandInvoker = new CommandInvoker(outputDirectory, printer);
                    commandInvoker.invoke(cmakeCommand.toArray(new String[cmakeCommand.size()]));
                }

                // Open the generated project file

                if (!Main.batchMode()) {
                    File fileToOpen = outputDirectory;

                    if (!build) {
                        if (Utility.IS_WINDOWS) {
                            File file = new File(outputDirectory, String.format("%s.sln", targetName));
                            if (file.exists() && !file.isDirectory())
                                fileToOpen = file;
                        }
                    }

                    try {
                        Desktop.getDesktop().open(fileToOpen);
                    } catch (IOException e) {
                        e.printStackTrace(System.err);
                        if (fileToOpen != outputDirectory) {
                            try {
                                Desktop.getDesktop().open(fileToOpen);
                            } catch (IOException ee) {
                                ee.printStackTrace(System.err);
                            }
                        }
                    }
                }
            }
        } finally {
            targetName = null;
            outputDirectory = null;
            defines = null;
            sourceFiles = null;
            headerFiles = null;
            thirdPartySourceFiles = null;
            thirdPartyHeaderFiles = null;
            headerPaths = null;
            thirdPartyHeaderPaths = null;
            projectFiles = null;
            this.project = null;
        }
    }

    private void writeCMakeLists(BuildTool selectedBuildTool, String selectedBuildType, String qt5Path)
    {
        // Build list of ADD_DEFINITIONS() commands

        StringBuilder definitions = new StringBuilder();
        if (!defines.isEmpty()) {
            definitions.append("add_definitions(\n");
            for (Map.Entry<String, String> define : defines.entrySet()) {
                String value = define.getValue().replace("\\", "\\\\").replace("\"", "\\\"");
                definitions.append(String.format("    \"-D%s=%s\"\n", cmakeEscape(define.getKey()), cmakeEscape(value)));
            }
            definitions.append(")");
        }

        // Build list of INCLUDE_DIRECTORIES() commands

        StringBuilder includeDirectories = new StringBuilder();
        if (!headerPaths.isEmpty()) {
            includeDirectories.append("include_directories(\n");
            for (File directory : headerPaths) {
                String relativePath = Utility.getRelativePath(new File(outputDirectory, SOURCE_DIRECTORY), directory);
                includeDirectories.append(String.format("    \"%s\"\n", cmakeEscapePath(relativePath)));
            }
            includeDirectories.append(")\n");
        }
        if (!thirdPartyHeaderPaths.isEmpty()) {
            includeDirectories.append("include_directories(SYSTEM\n");
            for (File directory : thirdPartyHeaderPaths) {
                String relativePath = Utility.getRelativePath(new File(outputDirectory, SOURCE_DIRECTORY), directory);
                includeDirectories.append(String.format("    \"%s\"\n", cmakeEscapePath(relativePath)));
            }
            includeDirectories.append(")\n");
        }

        // Build list of project files

        Map<String, List<String>> projectFileGroups = new LinkedHashMap<>();
        StringBuilder projectFileList = new StringBuilder();
        projectFileList.append(String.format("\"%s\"\n", cmakeEscapePath(
            Utility.getCanonicalPath(new File(project.projectDirectory(), ProjectFileParser.PROJECT_FILE_NAME)))));
        for (File projectDirectory : projectFiles) {
            List<String> items = new ArrayList<>();
            File projectFile = new File(projectDirectory, ProjectFileParser.PROJECT_FILE_NAME);
            extractSourceFileRelativePaths(outputDirectory, projectFile, items, projectFileGroups);
            projectFileList.append(String.format("        \"%s\"\n", cmakeEscapePath(items.get(0))));
        }

        // Build list of SOURCE_GROUP() commands for source files

        StringBuilder projectFileGroupsString = new StringBuilder();
        for (Map.Entry<String, List<String>> sourceGroup : projectFileGroups.entrySet()) {
            if (!sourceGroup.getValue().isEmpty()) {
                String groupName = sourceGroup.getKey().replace("/", "\\");
                while (groupName.startsWith("..\\"))
                    groupName = groupName.substring(3);
                projectFileGroupsString.append(String.format("source_group(\"%s\" FILES\n", cmakeEscape(groupName)));
                for (String file : sourceGroup.getValue())
                    projectFileGroupsString.append(String.format("    \"%s\"\n", cmakeEscapePath(file)));
                projectFileGroupsString.append(")\n\n");
            }
        }

        // Build list of configuration options

        StringBuilder projectOptions = new StringBuilder();
        for (Map.Entry<String, String> it : project.configurationOptions().entrySet()) {
            projectOptions.append(String.format("\"%s=%s\"\n        ",
                cmakeEscape(it.getKey()), cmakeEscape(it.getValue())));
        }

        // Build list of source files and SOURCE_GROUPs for them

        Map<String, List<String>> sourceGroups = new LinkedHashMap<>();
        List<String> sourcePaths = new ArrayList<>();
        List<String> headerPaths = new ArrayList<>();
        List<String> thirdPartySourcePaths = new ArrayList<>();
        List<String> thirdPartyHeaderPaths = new ArrayList<>();

        enumerateSourceFiles(sourceFiles, sourcePaths, sourceGroups);
        enumerateSourceFiles(headerFiles, headerPaths, sourceGroups);
        enumerateSourceFiles(thirdPartySourceFiles, thirdPartySourcePaths, sourceGroups);
        enumerateSourceFiles(thirdPartyHeaderFiles, thirdPartyHeaderPaths, sourceGroups);

        // Write root CMakeLists.txt

        FileBuilder builder = new FileBuilder(outputDirectory, "CMakeLists.txt");
        writeAutoGeneratedHeader(builder);

        Map<String, String> options = new LinkedHashMap<>();
        options.put("target_name", cmakeEscape(targetName));
        options.put("zbt_generator", cmakeEscape(NAME));
        options.put("zbt_java_executable", cmakeEscapePath(Utility.getJavaExecutable()));
        options.put("zbt_jar",
            cmakeEscapePath(Utility.getRelativePath(project.projectDirectory(), Utility.getApplicationJarFile())));
        options.put("zbt_project_directory", cmakeEscapePath(Utility.getCanonicalPath(project.projectDirectory())));
        if (selectedBuildTool == null)
            options.put("zbt_cmake_build_tool", "");
        else {
            options.put("zbt_cmake_build_tool",
                String.format("--cmake-build-tool \"%s\"", cmakeEscapePath(selectedBuildTool.title)));
        }
        if (selectedBuildType == null)
            options.put("zbt_cmake_build_type", "");
        else {
            options.put("zbt_cmake_build_type",
                String.format("--cmake-build-type \"%s\"", cmakeEscapePath(selectedBuildType)));
        }
        if (qt5Path == null)
            options.put("zbt_cmake_qt5_path", "");
        else
            options.put("zbt_cmake_qt5_path", String.format("--cmake-qt5-path \"%s\"", cmakeEscapePath(qt5Path)));
        options.put("zbt_project_files", projectFileList.toString());
        options.put("zbt_project_options", projectOptions.toString());
        options.put("source_groups", projectFileGroupsString.toString());
        rootTemplate.emit(builder, options);

        builder.commit(project.database());

        // Write src/CMakeLists.txt

        builder = new FileBuilder(outputDirectory, String.format("%s/CMakeLists.txt", SOURCE_DIRECTORY));
        writeAutoGeneratedHeader(builder);

        options = new HashMap<>();
        options.put("target_name", cmakeEscape(targetName));
        options.put("use_qt5", useQt5 ? "YES" : "NO");
        options.put("use_opengl", useOpenGL ? "YES" : "NO");
        options.put("defines", definitions.toString());
        options.put("include_directories", includeDirectories.toString());
        srcTemplate.emit(builder, options);

        builder.commit(project.database());

        // Write SourceFiles.cmake

        builder = new FileBuilder(outputDirectory, String.format("%s/SourceFiles.cmake", SOURCE_DIRECTORY));
        writeAutoGeneratedHeader(builder);
        writeSourcePaths(builder, "source_files", sourcePaths);
        writeSourcePaths(builder, "header_files", headerPaths);
        writeSourcePaths(builder, "third_party_source_files", thirdPartySourcePaths);
        writeSourcePaths(builder, "third_party_header_files", thirdPartyHeaderPaths);
        builder.commit(project.database());

        // Write SourceGroups.cmake

        builder = new FileBuilder(outputDirectory, String.format("%s/SourceGroups.cmake", SOURCE_DIRECTORY));
        writeAutoGeneratedHeader(builder);

        for (Map.Entry<String, List<String>> sourceGroup : sourceGroups.entrySet()) {
            if (!sourceGroup.getValue().isEmpty()) {
                String groupName = sourceGroup.getKey().replace("/", "\\");
                while (groupName.startsWith("..\\"))
                    groupName = groupName.substring(3);
                builder.append(String.format("source_group(\"%s\" FILES\n", cmakeEscape(groupName)));
                for (String file : sourceGroup.getValue())
                    builder.append(String.format("    \"%s\"\n", cmakeEscapePath(file)));
                builder.append(")\n\n");
            }
        }

        builder.commit(project.database());
    }

    private void writeAutoGeneratedHeader(FileBuilder builder)
    {
        builder.append('\n');
        builder.append("# ------------------------------------------------------\n");
        builder.append("# THIS IS AN AUTOMATICALLY GENERATED FILE. DO NOT EDIT!\n");
        builder.append("# ------------------------------------------------------\n");
        builder.append('\n');
    }

    private void writeSourcePaths(FileBuilder builder, String variableName, List<String> sourcePaths)
    {
        if (sourcePaths.isEmpty()) {
            builder.append(String.format("set(%s)\n", variableName));
        } else {
            builder.append(String.format("set(%s\n", variableName));
            for (String path : sourcePaths)
                builder.append(String.format("    \"%s\"\n", cmakeEscapePath(path)));
            builder.append(")\n");
        }
    }

    public static boolean isSourceFile(File file)
    {
        final String[] extensions = new String[]{ ".c", ".cc", ".cpp", ".cxx" };
        return Utility.fileHasExtension(file, extensions);
    }

    public static boolean isHeaderFile(File file)
    {
        final String[] extensions = new String[]{ ".h", ".hh", ".hpp", ".hxx", ".inl" };
        return Utility.fileHasExtension(file, extensions);
    }

    private void enumerateSourceFiles(List<File> inFiles, List<String> outPaths, Map<String, List<String>> outGroups)
    {
        File baseDirectory = new File(outputDirectory, SOURCE_DIRECTORY);
        for (File source : inFiles)
            extractSourceFileRelativePaths(baseDirectory, Utility.getCanonicalFile(source), outPaths, outGroups);
    }

    private void extractSourceFileRelativePaths(File baseDirectory, File file, List<String> outPaths,
        Map<String, List<String>> outGroups)
    {
        String path = Utility.getRelativePath(baseDirectory, file);

        if (outPaths != null)
            outPaths.add(path);

        String sourceGroup = Utility.getRelativePath(project.projectDirectory(), file.getParentFile());
        List<String> list = outGroups.get(sourceGroup);
        if (list == null) {
            list = new ArrayList<>();
            outGroups.put(sourceGroup, list);
        }
        list.add(path);
    }

    public static boolean isValidBuildTool(String name)
    {
        return name != null && buildTools().get(name) != null;
    }

    public static boolean isValidBuildType(String name)
    {
        return "Debug".equals(name) || "Release".equals(name)
            || "MinSizeRel".equals(name) || "RelWithDebInfo".equals(name);
    }

    public static boolean isValidQt5Directory(String path)
    {
        File file = new File(new File(path), "lib/cmake/Qt5/Qt5Config.cmake");
        if (file.exists() && !file.isDirectory())
            return true;

        file = new File(new File(path), "Qt5/Qt5Config.cmake");
        return (file.exists() && !file.isDirectory());
    }

    private static String findCMakeExecutable()
    {
        String path = Utility.resolveExecutable("cmake");
        if (path == null) {
            if (Utility.IS_WINDOWS) {
                String programFiles = System.getenv("ProgramFiles");
                if (programFiles == null)
                    programFiles = "C:/Program Files";
                File file = new File(String.format("%s/CMake/bin", programFiles));
                if (file.exists())
                    return Utility.getCanonicalPath(file);

                String programFilesX86 = System.getenv("ProgramFiles(x86)");
                if (programFilesX86 == null)
                    programFilesX86 = "C:/Program Files (x86)";
                file = new File(String.format("%s/CMake/bin", programFilesX86));
                if (file.exists())
                    return Utility.getCanonicalPath(file);
            }

            if (Utility.IS_OSX) {
                File file = new File("/Applications/CMake.app/Contents/bin/cmake");
                if (file.exists())
                    return Utility.getCanonicalPath(file);
            }
        }
        return path;
    }

    private String findQt5InstallationDirectory(BuildTool buildTool)
    {
        if (Utility.IS_WINDOWS) {
            for (String dir : new String[]{ "C:/Qt", "C:/Qt5" }) {
                File qtDir = new File(dir);
                if (!qtDir.exists() || !qtDir.isDirectory())
                    continue;

                for (String subdir : qtDir.list()) {
                    if (!subdir.matches("^[0-9]+\\.[0-9]+(\\.[0-9]+)?$"))
                        continue;

                    File qtVersionDir = new File(qtDir, subdir);
                    if (!qtVersionDir.isDirectory())
                        continue;

                    String pattern = null;
                    switch (buildTool)
                    {
                    case UNIX_MAKEFILES: break;
                    case XCODE: break;
                    case MINGW32: pattern = "^mingw[0-9]+_32$"; break;
                    case MINGW64: pattern = "^mingw[0-9]+_64$"; break;
                    case VS2013_WIN32: pattern = "^msvc2013$"; break;
                    case VS2013_WIN64: pattern = "^msvc2013_64$"; break;
                    case VS2015_WIN32: pattern = "^msvc2015$"; break;
                    case VS2015_WIN64: pattern = "^msvc2015_64$"; break;
                    }
                    if (pattern == null)
                        continue;

                    for (String subdir2 : qtVersionDir.list()) {
                        if (!subdir2.matches(pattern))
                            continue;

                        File qtPrefixPath = new File(qtVersionDir, subdir2);
                        if (!qtPrefixPath.isDirectory())
                            continue;

                        if (isCMakePrefixPath(qtPrefixPath))
                            return Utility.getCanonicalPath(qtPrefixPath);
                    }
                }
            }
        }

        return null;
    }

    private boolean isCMakePrefixPath(File path)
    {
        File cmakeFile = new File(path, "lib/cmake/Qt5/Qt5Config.cmake");
        return (cmakeFile.exists() && !cmakeFile.isDirectory());
    }

    private void updateUI()
    {
        BuildTool selectedBuildTool = selectedBuildTool();
        buildTypeCombo.getParent().setVisible(selectedBuildTool != null && selectedBuildTool.acceptsBuildType);
    }

    private BuildTool selectedBuildTool()
    {
        String key = (String)buildToolCombo.getSelectedItem();
        if (key == null)
            return null;
        return buildTools().get(key);
    }

    private String selectedBuildType()
    {
        return (String)buildTypeCombo.getSelectedItem();
    }

    public static String cmakeEscape(String string)
    {
        StringBuilder builder = new StringBuilder();
        for (int i = 0; i < string.length(); i++) {
            char ch = string.charAt(i);
            switch (ch)
            {
            case '\\': builder.append("\\\\"); break;
            case '"': builder.append("\\\""); break;
            default: builder.append(ch); break;
            }
        }
        return builder.toString();
    }

    public static String cmakeEscapePath(String string)
    {
        if (!Utility.IS_WINDOWS)
            return cmakeEscape(string);

        StringBuilder builder = new StringBuilder();
        for (int i = 0; i < string.length(); i++) {
            char ch = string.charAt(i);
            switch (ch)
            {
            case '\\': builder.append("/"); break;
            case '"': builder.append("\\\""); break;
            default: builder.append(ch); break;
            }
        }
        return builder.toString();
    }

    private static String cmakeMinGWGenerator;
    private static String cmakeMinGWGenerator()
    {
        if (cmakeMinGWGenerator == null)
            cmakeMinGWGenerator = (Utility.resolveExecutable("sh") != null ? "MSYS Makefiles" : "MinGW Makefiles");
        return cmakeMinGWGenerator;
    }

    private static Map<String, BuildTool> buildTools;
    private static Map<String, BuildTool> buildTools()
    {
        if (buildTools == null) {
            Map<String, BuildTool> g = new LinkedHashMap<>();

            if (Utility.IS_OSX) {
                g.put(BuildTool.XCODE.title, BuildTool.XCODE);
            }

            if (Utility.IS_WINDOWS) {
                g.put(BuildTool.VS2013_WIN32.title, BuildTool.VS2013_WIN32);
                g.put(BuildTool.VS2013_WIN64.title, BuildTool.VS2013_WIN64);
                g.put(BuildTool.VS2015_WIN32.title, BuildTool.VS2015_WIN32);
                g.put(BuildTool.VS2015_WIN64.title, BuildTool.VS2015_WIN64);
                g.put(BuildTool.MINGW32.title, BuildTool.MINGW32);
                g.put(BuildTool.MINGW64.title, BuildTool.MINGW64);
            }

            if (!Utility.IS_WINDOWS)
                g.put(BuildTool.UNIX_MAKEFILES.title, BuildTool.UNIX_MAKEFILES);

            buildTools = g;
        }
        return buildTools;
    }
}
