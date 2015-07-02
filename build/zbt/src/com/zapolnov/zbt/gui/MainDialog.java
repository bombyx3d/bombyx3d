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

import com.zapolnov.zbt.generators.Generator;
import com.zapolnov.zbt.project.Project;
import com.zapolnov.zbt.utility.GuiUtility;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Rectangle;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.util.Map;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ScrollPaneConstants;

public class MainDialog extends JDialog
{
    public static final String TITLE = "Generate Project Files";
    public static final String BUTTON_TITLE = "Generate";

    public static final int PREFERRED_WIDTH = GuiUtility.LABEL_PREFERRED_WIDTH +
        GuiUtility.COMBOBOX_PREFERRED_WIDTH + 70;
    public static final int PREFERRED_HEIGHT = 250;

    private final Project project;
    private final ProjectConfigurationPanel projectConfigurationPanel;

    public MainDialog(JFrame parent, Project project, Generator generator, Map<String, String> options)
    {
        super(parent, TITLE, true);

        this.project = project;

        setPreferredSize(new Dimension(PREFERRED_WIDTH, PREFERRED_HEIGHT));
        addComponentListener(new ComponentAdapter() {
             @Override public void componentResized(ComponentEvent e) {
                 Rectangle bounds = e.getComponent().getBounds();
                 setPreferredSize(new Dimension(bounds.width, bounds.height));
             }
        });

        setDefaultCloseOperation(DISPOSE_ON_CLOSE);

        JPanel contentPanel = new JPanel(new BorderLayout());
        contentPanel.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
        setContentPane(contentPanel);

        projectConfigurationPanel = new ProjectConfigurationPanel(project, generator, options);
        projectConfigurationPanel.addChangeListener(this::pack);

        JPanel wrapper = new JPanel();
        wrapper.add(projectConfigurationPanel);

        JScrollPane scrollArea = new JScrollPane(wrapper);
        scrollArea.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 0));
        scrollArea.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
        scrollArea.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
        contentPanel.add(scrollArea, BorderLayout.CENTER);

        JPanel buttonPanel = new JPanel();
        buttonPanel.setBorder(BorderFactory.createEmptyBorder(0, 5, 0, 0));
        contentPanel.add(buttonPanel, BorderLayout.PAGE_END);

        JButton button = new JButton(BUTTON_TITLE);
        button.addActionListener(e -> generateProject());
        buttonPanel.add(button, BorderLayout.CENTER);

        pack();
        setLocationRelativeTo(getParent());
    }

    private void generateProject()
    {
        try {
            Generator generator;
            Map<String, String> options;

            project.database().commit();

            try {
                generator = projectConfigurationPanel.selectedGenerator();
                options = projectConfigurationPanel.selectedOptions();

                if (!projectConfigurationPanel.validateAndSaveOptions()) {
                    project.database().rollbackSafe();
                    return;
                }

                project.database().commit();
            } catch (Throwable t) {
                project.database().rollbackSafe();
                throw t;
            }

            project.build(generator, options);
        } catch (Throwable t) {
            FatalErrorDialog dialog = new FatalErrorDialog(this, t);
            dialog.setVisible(true);
        }
    }

    public static void run(Project project, Generator generator, Map<String, String> options)
    {
        JFrame dummyFrame = new DummyFrame(TITLE);
        MainDialog mainDialog = new MainDialog(dummyFrame, project, generator, options);
        mainDialog.setVisible(true);
    }
}
