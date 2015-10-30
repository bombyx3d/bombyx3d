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
#include "style.h"
#include <QApplication>
#include <QPalette>
#include <QStyleFactory>

void B3D::qtInitDarkStyle()
{
    qApp->setStyle(QStyleFactory::create("Fusion"));

    QColor base(43, 43, 43);
    QColor background(53, 53, 53);
    QColor foreground(230, 230, 230);
    QColor foreground_disabled(128, 128, 128);
    QColor highlight(142, 45, 197);
    QColor bright(255, 255, 255);

    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window, background);
    pal.setColor(QPalette::WindowText, foreground);
    pal.setColor(QPalette::Disabled, QPalette::WindowText, foreground_disabled);
    pal.setColor(QPalette::Base, base);
    pal.setColor(QPalette::AlternateBase, background);
    pal.setColor(QPalette::ToolTipBase, background);
    pal.setColor(QPalette::ToolTipText, foreground);
    pal.setColor(QPalette::Text, foreground);
    pal.setColor(QPalette::Disabled, QPalette::Text, foreground_disabled);
    pal.setColor(QPalette::Button, background);
    pal.setColor(QPalette::ButtonText, foreground);
    pal.setColor(QPalette::Disabled, QPalette::ButtonText, foreground_disabled);
    pal.setColor(QPalette::BrightText, bright);
    pal.setColor(QPalette::Highlight, highlight);
    pal.setColor(QPalette::HighlightedText, foreground);
    pal.setColor(QPalette::Disabled, QPalette::HighlightedText, foreground_disabled);
    pal.setColor(QPalette::Disabled, QPalette::Light, foreground_disabled.darker());
    qApp->setPalette(pal);
}
