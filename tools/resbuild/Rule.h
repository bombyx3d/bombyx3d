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
#include <QDomElement>
#include <QObject>

class Project;
class QListWidgetItem;

class Rule : public QObject
{
    Q_OBJECT

public:
    QListWidgetItem* listWidgetItem = nullptr;
    bool isNewlyCreatedRule = true;

    explicit Rule(Project* project);
    Rule(Project* project, const BuilderPtr& builder);
    ~Rule();

    Project* project() const { return m_Project; }
    const BuilderPtr& builder() const { return m_Builder; }

    const QString& name() const { return m_Name; }
    Q_SLOT void setName(const QString& name);

    QWidget* createEditor(QWidget* parent = nullptr);

    bool load(const QDomElement& element, QString* errorMessage = nullptr);
    bool save(QDomElement& element, QString* errorMessage = nullptr);

private:
    Project* m_Project;
    BuilderPtr m_Builder;
    QString m_Name;
};
