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

import com.zapolnov.zbt.utility.CommandInvoker;
import java.awt.BorderLayout;
import java.awt.Container;
import java.lang.reflect.InvocationTargetException;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.ScrollPaneConstants;
import javax.swing.SwingUtilities;
import javax.swing.text.DefaultCaret;

public final class ConsoleDialog extends JDialog implements CommandInvoker.Printer
{
    public static final String TITLE = "Console";

    private final JTextArea textArea;
    public final JButton button;

    public ConsoleDialog(JDialog parent)
    {
        super(parent);
        setTitle(TITLE);

        textArea = new JTextArea();
        textArea.setEditable(false);
        textArea.setLineWrap(true);
        textArea.setWrapStyleWord(true);
        textArea.setRows(25);
        textArea.setColumns(80);

        JScrollPane scrollArea = new JScrollPane(textArea);
        scrollArea.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 0));
        scrollArea.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
        scrollArea.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
        getContentPane().add(scrollArea, BorderLayout.CENTER);

        JPanel buttonPanel = new JPanel();
        buttonPanel.setBorder(BorderFactory.createEmptyBorder(0, 5, 0, 0));
        add(buttonPanel, BorderLayout.PAGE_END);

        button = new JButton("Close");
        button.addActionListener(e -> setVisible(false));
        buttonPanel.add(button, BorderLayout.CENTER);

        DefaultCaret caret = (DefaultCaret) textArea.getCaret();
        caret.setUpdatePolicy(DefaultCaret.ALWAYS_UPDATE);

        pack();
    }

    public void print(final String message)
    {
        try
        {
            SwingUtilities.invokeAndWait(() -> {
                textArea.append(message);
                Container parent = textArea;
                for (;;) {
                    parent = parent.getParent();
                    if (parent == null)
                        break;
                    parent.validate();
                }

                if (!isVisible())
                    setVisible(true);
            });
        }
        catch (InterruptedException|InvocationTargetException e)
        {
            e.printStackTrace();
        }
    }
}
