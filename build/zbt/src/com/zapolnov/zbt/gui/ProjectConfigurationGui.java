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
package com.zapolnov.zbt.gui;

import com.zapolnov.zbt.Main;
import com.zapolnov.zbt.ProjectBuilder;
import com.zapolnov.zbt.generators.Generator;
import com.zapolnov.zbt.project.Project;
import com.zapolnov.zbt.project.ProjectVisitor;
import com.zapolnov.zbt.project.directive.EnumerationDirective;
import com.zapolnov.zbt.project.directive.SelectorDirective;
import com.zapolnov.zbt.utility.Database;
import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Stack;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;

public final class ProjectConfigurationGui extends ProjectVisitor
{
    private final static class Enumeration
    {
        public final JLabel label;
        public final JComboBox<String> comboBox;
        public final String[] values;

        public Enumeration(JLabel label, JComboBox<String> comboBox, String[] values)
        {
            this.label = label;
            this.comboBox = comboBox;
            this.values = values;
        }
    }

    private final Project project;
    private final ProjectBuilder projectBuilder;
    private final JFrame frame;
    private final CMakeOptionsPanel cmakeOptionsPanel;
    private final List<JLabel> labels = new ArrayList<>();
    private final List<JComponent> editors = new ArrayList<>();
    private final Map<String, Enumeration> enumerations = new HashMap<>();

    private ProjectConfigurationGui(ProjectBuilder projectBuilder)
    {
        this.projectBuilder = projectBuilder;
        this.project = projectBuilder.readProjectFile();

        frame = new JFrame("Project Configuration");
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        // Enumerate root level options
        project.visitDirectives(this);

        // Enumerate nested options
        project.visitDirectives(new ProjectVisitor() {
            @Override public void visitSelector(SelectorDirective directive) {
                directive.visitDirectives(ProjectConfigurationGui.this);
            }
        });

        cmakeOptionsPanel = new CMakeOptionsPanel();
        frame.getContentPane().add(cmakeOptionsPanel, BorderLayout.PAGE_START);

        JPanel optionsPanel = new JPanel();
        optionsPanel.setLayout(new GridLayout(labels.size(), 2));
        optionsPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        for (int i = 0; i < labels.size(); i++) {
            optionsPanel.add(labels.get(i), BorderLayout.WEST);
            optionsPanel.add(editors.get(i), BorderLayout.EAST);
        }
        frame.getContentPane().add(optionsPanel, BorderLayout.CENTER);

        updateOptionsVisibility();

        JPanel buttonsBox = new JPanel();
        buttonsBox.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
        frame.getContentPane().add(buttonsBox, BorderLayout.PAGE_END);

        JButton button = new JButton("Generate project");
        button.addActionListener(e -> ProjectConfigurationGui.this.generateProject());
        buttonsBox.add(button);

        frame.pack();
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }

    private void setSelectedTarget(String platform, String compiler)
    {
        cmakeOptionsPanel.setSelection(platform, compiler);
    }

    @Override public void visitEnumeration(EnumerationDirective directive)
    {
        JLabel label = new JLabel(directive.description());
        labels.add(label);

        JPanel panel = new JPanel();
        panel.setBorder(BorderFactory.createEmptyBorder(5, 10, 5, 0));
        panel.setLayout(new GridLayout(1, 1));
        editors.add(panel);

        Collection<String> enumValues = directive.values().values();
        String[] items = enumValues.toArray(new String[enumValues.size()]);

        Collection<String> enumNames = directive.values().keySet();
        String[] values = enumNames.toArray(new String[enumNames.size()]);

        JComboBox<String> comboBox = new JComboBox<>(items);
        comboBox.addActionListener(e -> updateOptionsVisibility());
        panel.add(comboBox, BorderLayout.EAST);

        enumerations.put(directive.name(), new Enumeration(label, comboBox, values));
    }

    private void generateProject()
    {
        try {
            Generator generator = cmakeOptionsPanel.createSelectedGenerator();

            projectBuilder.database().setOption(Database.TARGET_PLATFORM_OPTION, cmakeOptionsPanel.selectedPlatform());
            projectBuilder.database().setOption(Database.TARGET_COMPILER_OPTION, cmakeOptionsPanel.selectedCompiler());
            projectBuilder.database().commit();

            projectBuilder.buildProject(generator);
        } catch (Throwable t) {
            t.printStackTrace(System.err);
            String message = String.format("%s: %s", t.getClass().getName(), t.getMessage());
            JOptionPane.showMessageDialog(frame, message, "Fatal error", JOptionPane.ERROR_MESSAGE);
        }
    }

    private void updateOptionsVisibility()
    {
        final Stack<Boolean> visible = new Stack<>();
        visible.push(true);
        project.visitDirectives(new ProjectVisitor() {
            @Override public void visitEnumeration(EnumerationDirective directive) {
                Enumeration enumeration = enumerations.get(directive.name());
                if (enumeration != null) {
                    boolean thisVisible = visible.peek();
                    enumeration.label.setVisible(thisVisible);
                    enumeration.comboBox.setVisible(thisVisible);
                }
            }
            @Override public void visitSelector(SelectorDirective directive) {
                boolean thisVisible = false;
                Enumeration enumeration = enumerations.get(directive.enumerationName());
                if (enumeration != null) {
                    int index = enumeration.comboBox.getSelectedIndex();
                    if (index >= 0 && index < enumeration.values.length) {
                        String value = enumeration.values[index];
                        if (directive.enumerationValue().equals(value))
                            thisVisible = true;
                    }
                }
                visible.push(visible.peek() && thisVisible);
                try {
                    directive.visitDirectives(this);
                } finally {
                    visible.pop();
                }
            }
        });
    }

    public static void run(final ProjectBuilder projectBuilder, final String platform, final String compiler)
    {
        ProjectConfigurationGui gui = new ProjectConfigurationGui(projectBuilder);
        if (platform != null || compiler != null)
            gui.setSelectedTarget(platform, compiler);
        else {
            String savedPlatform = projectBuilder.database().getOption(Database.TARGET_PLATFORM_OPTION);
            String savedCompiler = projectBuilder.database().getOption(Database.TARGET_COMPILER_OPTION);
            if (savedPlatform != null && savedCompiler != null) {
                try {
                    gui.setSelectedTarget(savedPlatform, savedCompiler);
                } catch (Throwable t) {
                    t.printStackTrace();
                }
            }
        }
    }
}
