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
import com.zapolnov.zbt.project.parser.ProjectDirectiveList;
import com.zapolnov.zbt.project.parser.AbstractProjectDirectiveVisitor;
import com.zapolnov.zbt.project.parser.directives.EnumerationDirective;
import com.zapolnov.zbt.project.parser.directives.GeneratorSelectorDirective;
import com.zapolnov.zbt.project.parser.directives.ImportDirective;
import com.zapolnov.zbt.project.parser.directives.RootProjectSelectorDirective;
import com.zapolnov.zbt.project.parser.directives.SelectorDirective;
import com.zapolnov.zbt.utility.Database;
import com.zapolnov.zbt.utility.GuiUtility;
import java.awt.Container;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import javax.swing.BoxLayout;
import javax.swing.JComboBox;
import javax.swing.JPanel;

public final class ProjectConfigurationPanel extends JPanel
{
    public static final String GENERATOR_LABEL = "Generator:";

    public interface Listener
    {
        void onProjectConfigurationPanelChanged();
    }

    private final Project project;
    private final JComboBox<String> generatorCombo;
    private final Map<Generator, JPanel> generatorSettingsPanels = new HashMap<>();
    private final Map<JComboBox<String>, EnumerationDirective> enumerationDirectives = new LinkedHashMap<>();
    private final Map<EnumerationDirective, JComboBox<String>> comboBoxes = new LinkedHashMap<>();
    private final Map<String, List<JComboBox<String>>> comboBoxesById = new LinkedHashMap<>();
    private final List<Container> widgetPanels = new ArrayList<>();
    private final List<Listener> listeners = new ArrayList<>();
    private final Map<String, String> defaultOptionValues;

    public ProjectConfigurationPanel(Project project, Generator defaultGenerator, Map<String, String> options)
    {
        this.project = project;
        this.defaultOptionValues = new LinkedHashMap<>(options);

        setLayout(new BoxLayout(this, BoxLayout.PAGE_AXIS));

        String defaultGeneratorName = (defaultGenerator != null ? defaultGenerator.name() : null);
        if (defaultGeneratorName == null)
            defaultGeneratorName = project.database().getOption(Database.OPTION_GENERATOR_NAME);
        generatorCombo = GuiUtility.createComboBox(this, GENERATOR_LABEL, Generator.allGenerators().keySet(), defaultGeneratorName);
        generatorCombo.addItemListener(e -> updateWidgetsVisibility());

        for (Generator generator : Generator.allGenerators().values()) {
            JPanel panel = generator.createSettingsPanel(project.database());
            if (panel != null) {
                add(panel);
                generatorSettingsPanels.put(generator, panel);
            }
        }

        createWidgets();
        updateWidgetsVisibility();
    }

    public void addChangeListener(Listener listener)
    {
        listeners.add(listener);
    }

    private void createWidgets()
    {
        createWidgets(project.directives());
    }

    private void createWidgets(ProjectDirectiveList directives)
    {
        directives.visitDirectives(new AbstractProjectDirectiveVisitor() {
            @Override public void visitEnumeration(EnumerationDirective directive) {
                createEnumerationWidget(directive);
            }
        });

        directives.visitDirectives(new AbstractProjectDirectiveVisitor() {
            @Override public void visitImport(ImportDirective directive) {
                createWidgets(directive.innerDirectives());
            }
            @Override public void visitSelector(SelectorDirective directive) {
                createWidgets(directive.innerDirectives());
            }
            @Override public void visitGeneratorSelector(GeneratorSelectorDirective directive) {
                directive.mapping().values().forEach(ProjectConfigurationPanel.this::createWidgets);
            }
            @Override public void visitRootProjectSelector(RootProjectSelectorDirective directive) {
                createWidgets(directive.innerDirectives());
            }
        });
    }

    private void createEnumerationWidget(EnumerationDirective directive)
    {
        JComboBox<String> comboBox = comboBoxes.get(directive);
        if (comboBox != null)
            return;

        Collection<String> values = directive.values().values();

        int selectedIndex = -1;
        String defaultValue = defaultOptionValues.get(directive.id());
        if (defaultValue == null)
            defaultValue = project.database().getOption(String.format(Database.PROJECT_OPTION_FORMAT, directive.id()));
        if (defaultValue == null)
            defaultValue = directive.defaultValue();
        if (defaultValue != null) {
            selectedIndex = getEnumerationSelectedIndex(directive, defaultValue);
            if (selectedIndex < 0 && !defaultValue.equals(directive.defaultValue())) {
                defaultValue = directive.defaultValue();
                if (defaultValue != null)
                    selectedIndex = getEnumerationSelectedIndex(directive, defaultValue);
            }
        }

        comboBox = GuiUtility.createComboBox(this, directive.title() + ':', values, selectedIndex);
        comboBox.addItemListener(e -> updateWidgetsVisibility());

        List<JComboBox<String>> list = comboBoxesById.get(directive.id());
        if (list == null) {
            list = new ArrayList<>();
            comboBoxesById.put(directive.id(), list);
        }
        list.add(comboBox);

        comboBoxes.put(directive, comboBox);
        enumerationDirectives.put(comboBox, directive);
        widgetPanels.add(comboBox.getParent());
    }

    private int getEnumerationSelectedIndex(EnumerationDirective directive, String defaultValue)
    {
        int index = 0;
        for (Map.Entry<String, String> item : directive.values().entrySet()) {
            if (defaultValue.equals(item.getKey()))
                return index;
            ++index;
        }
        return -1;
    }

    private void updateWidgetsVisibility()
    {
        Set<Container> visible = new HashSet<>();
        updateWidgetsVisibility(visible, project.directives());

        for (Container panel : widgetPanels)
            panel.setVisible(visible.contains(panel));

        Generator selectedGenerator = selectedGenerator();
        for (Map.Entry<Generator, JPanel> it : generatorSettingsPanels.entrySet())
            it.getValue().setVisible(it.getKey() == selectedGenerator);

        List<Listener> listenerList = new ArrayList<>(listeners);
        listenerList.forEach(ProjectConfigurationPanel.Listener::onProjectConfigurationPanelChanged);
    }

    private void updateWidgetsVisibility(final Set<Container> visible, ProjectDirectiveList directives)
    {
        directives.visitDirectives(new AbstractProjectDirectiveVisitor() {
            @Override public void visitEnumeration(EnumerationDirective directive) {
                visible.add(comboBoxes.get(directive).getParent());
            }
            @Override public void visitImport(ImportDirective directive) {
                updateWidgetsVisibility(visible, directive.innerDirectives());
            }
            @Override public void visitSelector(SelectorDirective directive) {
                List<JComboBox<String>> list = comboBoxesById.get(directive.enumerationID());
                for (JComboBox<String> comboBox : list) {
                    String selectedItem = getComboBoxSelectedEnumeration(comboBox);
                    if (selectedItem != null && directive.matchingValues().contains(selectedItem)) {
                        updateWidgetsVisibility(visible, directive.innerDirectives());
                        return;
                    }
                }
            }
            @Override public void visitGeneratorSelector(GeneratorSelectorDirective directive) {
                Generator selectedGenerator = selectedGenerator();
                if (selectedGenerator == null)
                    return;
                ProjectDirectiveList directives = directive.mapping().get(selectedGenerator.id());
                if (directives == null)
                    directives = directive.mapping().get(GeneratorSelectorDirective.DEFAULT);
                if (directives != null)
                    updateWidgetsVisibility(visible, directives);
            }
            @Override public void visitRootProjectSelector(RootProjectSelectorDirective directive) {
                if (directive.isTrue)
                    updateWidgetsVisibility(visible, directive.innerDirectives());
            }
        });
    }

    private String getComboBoxSelectedEnumeration(JComboBox<String> comboBox)
    {
        if (comboBox == null)
            return null;

        EnumerationDirective enumeration = enumerationDirectives.get(comboBox);
        if (enumeration == null)
            return null;

        int selectedIndex = comboBox.getSelectedIndex();
        if (selectedIndex < 0)
            return null;

        Iterator<String> valueIterator = enumeration.values().keySet().iterator();
        for (int i = 0; valueIterator.hasNext(); i++) {
            String value = valueIterator.next();
            if (i == selectedIndex)
                return value;
        }

        return null;
    }

    public Generator selectedGenerator()
    {
        String generatorName = (String)generatorCombo.getSelectedItem();
        return Generator.allGenerators().get(generatorName);
    }

    public Map<String, String> selectedOptions()
    {
        Map<String, String> options = new LinkedHashMap<>(defaultOptionValues);

        Set<String> visibleIDs = new HashSet<>();
        for (Map.Entry<EnumerationDirective, JComboBox<String>> it : comboBoxes.entrySet()) {
            if (it.getValue().isVisible())
                visibleIDs.add(it.getKey().id());
        }

        for (Map.Entry<EnumerationDirective, JComboBox<String>> it : comboBoxes.entrySet()) {
            String enumerationID = it.getKey().id();
            if (!visibleIDs.contains(enumerationID))
                options.remove(enumerationID);
            else {
                JComboBox<String> comboBox = it.getValue();
                if (comboBox.getParent().isVisible()) {
                    String value = getComboBoxSelectedEnumeration(comboBox);
                    if (value != null)
                        options.put(enumerationID, value);
                    else
                        options.remove(enumerationID);
                }
            }
        }

        return options;
    }

    public void validateAndSaveOptions()
    {
        Generator selectedGenerator = selectedGenerator();
        Map<String, String> selectedOptions = selectedOptions();

        if (selectedGenerator == null)
            throw new RuntimeException("No generator has been selected.");

        project.database().setOption(Database.OPTION_GENERATOR_NAME, selectedGenerator.name());
        for (Map.Entry<String, String> option : selectedOptions.entrySet()) {
            String key = String.format(Database.PROJECT_OPTION_FORMAT, option.getKey());
            project.database().setOption(key, option.getValue());
        }

        selectedGenerator.validateAndSaveSettings(project.database());
    }
}
