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

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.util.Arrays;
import java.util.Collection;
import javax.swing.BorderFactory;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;

public final class GuiUtility
{
    public static final int LABEL_PREFERRED_WIDTH = 200;
    public static final int COMBOBOX_PREFERRED_WIDTH = 200;

    private GuiUtility() {}

    public static JComboBox<String> createComboBox(Container container,
        String labelText, String[] items, String selectedItem)
    {
        return createComboBox(container, labelText, Arrays.asList(items), selectedItem);
    }

    public static JComboBox<String> createComboBox(Container container,
        String labelText, Collection<String> items, String selectedItem)
    {
        int selectedIndex = -1;
        if (selectedItem != null) {
            int index = 0;
            for (String item : items) {
                if (selectedItem.equals(item)) {
                    selectedIndex = index;
                    break;
                }
                ++index;
            }
        }
        return createComboBox(container, labelText, items, selectedIndex);
    }

    public static JComboBox<String> createComboBox(Container container,
        String labelText, Collection<String> items, int selectedIndex)
    {
        JPanel panel = new JPanel(new GridLayout(1, 2));
        panel.setBorder(BorderFactory.createEmptyBorder(0, 0, 5, 0));
        container.add(panel);

        JLabel label = new JLabel(labelText);
        label.setPreferredSize(new Dimension(LABEL_PREFERRED_WIDTH, label.getPreferredSize().height));
        panel.add(label, BorderLayout.WEST);

        JComboBox<String> comboBox = new JComboBox<>(items.toArray(new String[items.size()]));
        comboBox.setPreferredSize(new Dimension(COMBOBOX_PREFERRED_WIDTH, comboBox.getPreferredSize().height));
        comboBox.setEditable(false);
        panel.add(comboBox, BorderLayout.EAST);

        comboBox.setSelectedIndex(selectedIndex);

        return comboBox;
    }
}
