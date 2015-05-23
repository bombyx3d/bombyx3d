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
#include "Rule.h"
#include "Builder.h"
#include <vector>
#include <memory>
#include <QObject>

class Project : public QObject
{
    Q_OBJECT

public:
    explicit Project(QObject* parent = nullptr);
    ~Project();

    bool load(const QString& fileName, QString* errorMessage = nullptr);
    bool save(const QString& fileName, QString* errorMessage = nullptr);

    bool isModified() const { return m_Modified; }
    void setModified(bool flag = true) { m_Modified = flag; emit updateUI(); }

    uint64_t nextRuleId() { auto id = m_NextRuleID++; setModified(true); return id; }
    void createRule(const BuilderPtr& builder);
    void removeRule(Rule* rule);
    void removeRule(QListWidgetItem* item);
    Rule* ruleForItem(QListWidgetItem* item);

signals:
    void ruleCreated(Rule* rule);
    void ruleDeleted(Rule* rule);
    void updateUI();

private:
    bool m_Modified = false;
    std::vector<std::unique_ptr<Rule>> m_Rules;
    uint64_t m_NextRuleID = 1;
};
