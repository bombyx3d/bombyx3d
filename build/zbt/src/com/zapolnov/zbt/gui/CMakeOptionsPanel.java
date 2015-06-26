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
import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.GridLayout;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import javax.swing.BorderFactory;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class CMakeOptionsPanel extends JPanel
{
    private final List<JComboBox<String>> compilerCombos = new ArrayList<>();
    private final JComboBox<String> targetPlatformCombo;

    public CMakeOptionsPanel()
    {
        setLayout(new GridLayout(2, 2));
        setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        List<String> platformNames = new ArrayList<>();
        final JPanel compilerComboBoxPanel = new JPanel(new CardLayout());

        Map<String, Map<String, Generator.Factory>> generators = Generator.allGenerators();
        for (Map.Entry<String, Map<String, Generator.Factory>> it : generators.entrySet()) {
            platformNames.add(it.getKey());

            Set<String> compilers = it.getValue().keySet();
            JComboBox<String> comboBox = new JComboBox<>(compilers.toArray(new String[compilers.size()]));

            compilerCombos.add(comboBox);
            compilerComboBoxPanel.add(comboBox, it.getKey());
        }

        targetPlatformCombo = new JComboBox<>(platformNames.toArray(new String[platformNames.size()]));
        targetPlatformCombo.addItemListener(e -> {
            CardLayout layout = (CardLayout)compilerComboBoxPanel.getLayout();
            layout.show(compilerComboBoxPanel, (String)e.getItem());
        });

        add(new JLabel("Target platform"), BorderLayout.WEST);
        add(targetPlatformCombo, BorderLayout.EAST);

        add(new JLabel("Compiler/IDE"), BorderLayout.WEST);
        add(compilerComboBoxPanel, BorderLayout.EAST);
    }

    public void setSelection(String platform, String compiler)
    {
        Map<String, Map<String, Generator.Factory>> generators = Generator.allGenerators();
        int index = 0;
        for (Map.Entry<String, Map<String, Generator.Factory>> it : generators.entrySet()) {
            if (it.getKey().equals(platform)) {
                JComboBox<String> compilerCombo = compilerCombos.get(index);
                for (String jt : it.getValue().keySet()) {
                    if (jt.equals(compiler)) {
                        targetPlatformCombo.setSelectedItem(platform);
                        compilerCombo.setSelectedItem(compiler);
                        return;
                    }
                }
            }
            ++index;
        }

        throw new RuntimeException(String.format("Invalid platform/compiler: \"%s / %s\".", platform, compiler));
    }

    public String selectedPlatform()
    {
        return (String)targetPlatformCombo.getSelectedItem();
    }

    public String selectedCompiler()
    {
        int selectedTarget = targetPlatformCombo.getSelectedIndex();
        JComboBox<String> compilerCombo = compilerCombos.get(selectedTarget);
        return (String)compilerCombo.getSelectedItem();
    }

    public Generator createSelectedGenerator()
    {
        int selectedTarget = targetPlatformCombo.getSelectedIndex();
        JComboBox<String> compilerCombo = compilerCombos.get(selectedTarget);

        String platform = (String)targetPlatformCombo.getSelectedItem();
        String compiler = (String)compilerCombo.getSelectedItem();

        return Generator.allGenerators().get(platform).get(compiler).create();
    }
}
