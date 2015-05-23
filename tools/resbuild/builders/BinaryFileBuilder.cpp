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
#include "BinaryFileBuilder.h"
#include "Ui_BinaryFileBuilder.h"
#include <QWidget>
#include <QFileDialog>

static const QString g_InputFileAttribute = "inputFile";
static const QString g_OutputNameAttribute = "outputName";

BinaryFileBuilder::BinaryFileBuilder()
{
}

BinaryFileBuilder::~BinaryFileBuilder()
{
}

void BinaryFileBuilder::setInputFile(const QString& file)
{
    if (m_InputFile != file) {
        m_InputFile = file;
        emit inputFileChanged(file);
        emit setModified();
    }
}

void BinaryFileBuilder::setOutputName(const QString& name)
{
    if (m_OutputName != name) {
        m_OutputName = name;
        emit outputNameChanged(name);
        emit setModified();
    }
}

void BinaryFileBuilder::browseInputFile()
{
    QWidget* parent = qobject_cast<QWidget*>(sender());
    QString file = QFileDialog::getOpenFileName(parent, tr("Select input file"), m_InputFile, tr("All files (*)"));
    if (file.length() > 0)
        setInputFile(file);
}

QWidget* BinaryFileBuilder::createEditor(QWidget* parent)
{
    QWidget* editor = Builder::createEditor(parent);

    QWidget* widget = new QWidget(editor);
    editor->layout()->addWidget(widget);

    Ui_BinaryFileBuilder ui;
    ui.setupUi(widget);

    connect(ui.uiInputFileEdit, SIGNAL(textEdited(const QString&)), this, SLOT(setInputFile(const QString&)));
    connect(this, SIGNAL(inputFileChanged(const QString&)),ui.uiInputFileEdit, SLOT(setText(const QString&)));
    ui.uiInputFileEdit->setText(m_InputFile);

    connect(ui.uiOutputNameEdit, SIGNAL(textEdited(const QString&)), this, SLOT(setOutputName(const QString&)));
    connect(this, SIGNAL(outputNameChanged(const QString&)),ui.uiOutputNameEdit, SLOT(setText(const QString&)));
    ui.uiOutputNameEdit->setText(m_OutputName);

    connect(ui.uiInputBrowseButton, SIGNAL(clicked()), this, SLOT(browseInputFile()));

    return editor;
}

bool BinaryFileBuilder::load(const QDomElement& element, const QDir& projectDir, QString* errorMessage)
{
    if (!Builder::load(element, projectDir, errorMessage))
        return false;

    if (!element.hasAttribute(g_InputFileAttribute)) {
        if (errorMessage) {
            *errorMessage = tr("at line %1, column %2: missing attribute \"%4\" for element \"%3\" .")
                .arg(element.lineNumber()).arg(element.columnNumber()).arg(element.tagName()).arg(g_InputFileAttribute);
        }
        return false;
    }

    if (!element.hasAttribute(g_OutputNameAttribute)) {
        if (errorMessage) {
            *errorMessage = tr("at line %1, column %2: missing attribute \"%4\" for element \"%3\" .")
                .arg(element.lineNumber()).arg(element.columnNumber()).arg(element.tagName()).arg(g_OutputNameAttribute);
        }
        return false;
    }

    QString inputFile = projectDir.absoluteFilePath(element.attribute(g_InputFileAttribute));
    QString canonicalInputFile = QFileInfo(inputFile).canonicalFilePath();
    if (canonicalInputFile.length() > 0)
        inputFile = canonicalInputFile;

    setInputFile(inputFile);
    setOutputName(element.attribute(g_OutputNameAttribute));

    return true;
}

bool BinaryFileBuilder::save(QDomElement& element, const QDir& projectDir, QString* errorMessage)
{
    if (!Builder::save(element, projectDir, errorMessage))
        return false;

    element.setAttribute(g_InputFileAttribute, projectDir.relativeFilePath(m_InputFile));
    element.setAttribute(g_OutputNameAttribute, m_OutputName);

    return true;
}

bool BinaryFileBuilder::build(BuildState* state)
{
    // FIXME
    return true;
}
