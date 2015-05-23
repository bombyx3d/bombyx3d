/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com)
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

#pragma once
#include "Builder.h"

class BinaryFileBuilder : public Builder
{
    Q_OBJECT
    Z_BUILDER_XMLID("BinaryFileBuilder")
    Z_BUILDER_NAME(tr("Binary file"))
    Z_BUILDER_ICON(":/icons/fatcow_modified/compile_dark.png")

public:
    BinaryFileBuilder();
    ~BinaryFileBuilder();

    Q_SLOT void setInputFile(const QString& file);
    Q_SLOT void setOutputName(const QString& name);
    Q_SLOT void browseInputFile();

    QWidget* createEditor(QWidget* parent = nullptr) override;

    bool load(const QDomElement& element, const QDir& projectDir, QString* errorMessage) override;
    bool save(QDomElement& element, const QDir& projectDir, QString* errorMessage) override;

signals:
    void inputFileChanged(const QString& text);
    void outputNameChanged(const QString& text);

private:
    QString m_InputFile;
    QString m_OutputName;
};
