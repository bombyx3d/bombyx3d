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
#include "Project.h"
#include "FileUtil.h"
#include <QFile>
#include <QDomDocument>
#include <QDomElement>

static const QString g_RootElement = "ResourceBuilderProject";
static const QString g_RulesElement = "Rules";
static const QString g_RuleElement = "Rule";

Project::Project(QObject* parent)
    : QObject(parent)
{
    setModified(true);
}

Project::~Project()
{
}

bool Project::load(const QString& fileName, QString* errorMessage)
{
    m_Rules.clear();
    setModified(true);

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        if (errorMessage)
            *errorMessage = tr("Unable to open file \"%1\": %2").arg(fileName).arg(file.errorString());
        return false;
    }

    QDomDocument doc;
    QString xmlErrorMessage = tr("XML parse error.");
    int xmlErrorLine = 0, xmlErrorColumn = 0;
    if (!doc.setContent(&file, false, &xmlErrorMessage, &xmlErrorLine, &xmlErrorColumn)) {
        if (errorMessage) {
            *errorMessage = tr("Unable to parse file \"%1\": at line %2, column %3: %4")
                .arg(fileName).arg(xmlErrorLine).arg(xmlErrorColumn).arg(xmlErrorMessage);
        }
        return false;
    }

    QDomElement rootElement = doc.documentElement();
    if (rootElement.isNull() || rootElement.tagName() != g_RootElement) {
        if (errorMessage) {
            *errorMessage = tr("Unable to parse file \"%1\": root element is not \"%2\".")
                .arg(fileName).arg(g_RootElement);
        }
        return false;
    }

    for (QDomNode n = rootElement.firstChild(); !n.isNull(); n = n.nextSibling()) {
        QDomElement e = n.toElement();
        if (e.isNull())
            continue;

        if (e.tagName() == g_RulesElement) {
            for (QDomNode nn = e.firstChild(); !nn.isNull(); nn = nn.nextSibling()) {
                QDomElement ee = nn.toElement();
                if (ee.isNull())
                    continue;
                if (ee.tagName() == g_RuleElement) {
                    std::unique_ptr<Rule> rule(new Rule(this));
                    if (!rule->load(ee, errorMessage))
                        return false;
                    m_Rules.emplace_back(std::move(rule));
                } else {
                    if (errorMessage) {
                        *errorMessage = tr("Unable to parse file \"%1\": at line %2, column %3: unexpected element \"%4\".")
                            .arg(fileName).arg(ee.lineNumber()).arg(ee.columnNumber()).arg(ee.tagName());
                    }
                    return false;
                }
            }
        } else {
            if (errorMessage) {
                *errorMessage = tr("Unable to parse file \"%1\": at line %2, column %3: unexpected element \"%4\".")
                    .arg(fileName).arg(e.lineNumber()).arg(e.columnNumber()).arg(e.tagName());
            }
            return false;
        }
    }

    setModified(false);

    return true;
}

bool Project::save(const QString& fileName, QString* errorMessage)
{
    QDomDocument doc;

    QDomElement rootElement = doc.createElement(g_RootElement);
    doc.appendChild(rootElement);

    QDomElement rulesElement = doc.createElement(g_RulesElement);
    rootElement.appendChild(rulesElement);

    for (const auto& rule : m_Rules) {
        QDomElement ruleElement = doc.createElement(g_RuleElement);
        if (!rule->save(ruleElement, errorMessage))
            return false;
        rulesElement.appendChild(ruleElement);
    }

    if (!FileUtil::writeFile(fileName, doc.toByteArray(), true, errorMessage))
        return false;

    setModified(false);
    return true;
}
