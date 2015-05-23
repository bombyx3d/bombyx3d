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
#include "Rule.h"
#include "Builder.h"
#include "Project.h"
#include <cassert>
#include <QListWidgetItem>

static const QString g_BuilderElement = "Builder";
static const QString g_RuleNameAttribute = "name";
static const QString g_BuilderIdAttribute = "id";

Rule::Rule(Project* project)
    : QObject(project)
    , m_Project(project)
{
}

Rule::Rule(Project* project, const BuilderPtr& builder)
    : QObject(project)
    , m_Project(project)
    , m_Builder(builder)
    , m_Name(tr("Rule %1").arg(project->nextRuleId()))
{
}

Rule::~Rule()
{
}

void Rule::setName(const QString& name)
{
    m_Name = name;
    if (listWidgetItem)
        listWidgetItem->setText(name);
}

bool Rule::load(const QDomElement& element, QString* errorMessage)
{
    BuilderPtr builder;

    if (!element.hasAttribute(g_RuleNameAttribute)) {
        if (errorMessage) {
            *errorMessage = tr("at line %1, column %2: missing attribute \"%4\" for element \"%3\".")
                .arg(element.lineNumber()).arg(element.columnNumber()).arg(element.tagName()).arg(g_RuleNameAttribute);
        }
        return false;
    }

    QString name = element.attribute(g_RuleNameAttribute);
    for (QDomNode n = element.firstChild(); !n.isNull(); n = n.nextSibling()) {
        QDomElement e = n.toElement();
        if (e.isNull())
            continue;

        if (e.tagName() == g_BuilderElement) {
            if (builder) {
                if (errorMessage) {
                    *errorMessage = tr("at line %1, column %2: duplicate element \"%3\".")
                        .arg(e.lineNumber()).arg(e.columnNumber()).arg(e.tagName());
                }
                return false;
            }
            if (!e.hasAttribute(g_BuilderIdAttribute)) {
                if (errorMessage) {
                    *errorMessage = tr("at line %1, column %2: missing attribute \"%4\" for element \"%3\" .")
                        .arg(e.lineNumber()).arg(e.columnNumber()).arg(e.tagName()).arg(g_BuilderIdAttribute);
                }
                return false;
            }
            QString builderId = e.attribute(g_BuilderIdAttribute);
            auto factory = Builder::factoryWithXmlId(builderId);
            if (!factory) {
                if (errorMessage) {
                    *errorMessage = tr("at line %1, column %2: unknown builder \"%3\".")
                        .arg(e.lineNumber()).arg(e.columnNumber()).arg(builderId);
                }
                return false;
            }
            builder = factory->createBuilder();
            if (!builder) {
                if (errorMessage) {
                    *errorMessage = tr("at line %1, column %2: unable to create builder \"%3\".")
                        .arg(e.lineNumber()).arg(e.columnNumber()).arg(builderId);
                }
                return false;
            }
        } else {
            if (errorMessage) {
                *errorMessage = tr("at line %1, column %2: unexpected element \"%3\".")
                    .arg(e.lineNumber()).arg(e.columnNumber()).arg(e.tagName());
            }
            return false;
        }
    }

    if (!builder) {
        if (errorMessage) {
            *errorMessage = tr("at line %1, column %2: missing element \"%3\".")
                .arg(element.lineNumber()).arg(element.columnNumber()).arg(g_BuilderElement);
        }
        return false;
    }

    m_Name = name;
    m_Builder = std::move(builder);

    return true;
}

bool Rule::save(QDomElement& element, QString* errorMessage)
{
    element.setAttribute(g_RuleNameAttribute, m_Name);

    QDomElement builderElement = element.ownerDocument().createElement(g_BuilderElement);
    element.appendChild(builderElement);

    builderElement.setAttribute(g_BuilderIdAttribute, m_Builder->xmlId());
    if (!m_Builder->save(builderElement, errorMessage))
        return false;

    return true;
}
