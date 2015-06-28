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

import java.awt.BorderLayout;
import java.awt.Font;
import java.io.PrintWriter;
import java.io.StringWriter;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

public class FatalErrorDialog extends JDialog
{
    public static final String TITLE = "Fatal Error";
    public static final String BUTTON_TITLE = "OK";

    public FatalErrorDialog(JDialog parent, Throwable exception)
    {
        super(parent, TITLE, true);
        init(makeMessageForException(exception));
    }

    public FatalErrorDialog(JFrame parent, String message)
    {
        super(parent, TITLE, true);
        init(message);
    }

    private void init(String message)
    {
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);

        JPanel contentPanel = new JPanel(new BorderLayout());
        contentPanel.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
        setContentPane(contentPanel);

        JTextArea textArea = new JTextArea();
        textArea.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 12));
        textArea.setEditable(false);
        textArea.setLineWrap(false);
        textArea.setWrapStyleWord(true);
        textArea.setRows(10);
        textArea.append(message);
        textArea.setCaretPosition(0);

        JScrollPane scrollArea = new JScrollPane(textArea);
        scrollArea.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 0));
        contentPanel.add(scrollArea, BorderLayout.CENTER);

        JPanel buttonPanel = new JPanel();
        buttonPanel.setBorder(BorderFactory.createEmptyBorder(0, 5, 0, 0));
        contentPanel.add(buttonPanel, BorderLayout.PAGE_END);

        JButton button = new JButton(BUTTON_TITLE);
        button.addActionListener(e -> dispose());
        buttonPanel.add(button, BorderLayout.CENTER);

        pack();
        setLocationRelativeTo(getParent());
    }

    private static String makeMessageForException(Throwable exception)
    {
        StringWriter writer = new StringWriter();
        exception .printStackTrace(new PrintWriter(writer));
        return writer.toString();
    }

    public static void run(Throwable exception)
    {
        run(makeMessageForException(exception));
    }

    public static void run(String message)
    {
        JFrame dummyFrame = new DummyFrame(TITLE);
        FatalErrorDialog mainDialog = new FatalErrorDialog(dummyFrame, message);
        mainDialog.setVisible(true);
        System.exit(1);
    }
}
