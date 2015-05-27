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
#include "NewRuleDialog.h"
#include "../core/rules/BinaryFileRule.h"
#include <cassert>
#include <QPushButton>

static const std::string g_InvalidClassName;

NewRuleDialog::NewRuleDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);
    on_uiBuilderList_itemSelectionChanged();

    auto item = new QListWidgetItem(QIcon(":/icons/fatcow_modified/compile_dark.png"), tr("Binary File"), uiBuilderList);
    m_ClassNames.emplace(std::make_pair(item, BinaryFileRule::CLASS_NAME));
}

NewRuleDialog::~NewRuleDialog()
{
}

const std::string& NewRuleDialog::selectedClass() const
{
    QList<QListWidgetItem*> selectedItems = uiBuilderList->selectedItems();
    if (selectedItems.count() != 1)
        return g_InvalidClassName;

    auto it = m_ClassNames.find(selectedItems[0]);
    return (it != m_ClassNames.end() ? it->second : g_InvalidClassName);
}

void NewRuleDialog::on_uiBuilderList_itemSelectionChanged()
{
    QList<QListWidgetItem*> selectedItems = uiBuilderList->selectedItems();
    uiButtonBox->button(QDialogButtonBox::Ok)->setEnabled(selectedItems.count() == 1);
}

void NewRuleDialog::on_uiBuilderList_itemDoubleClicked(QListWidgetItem* item)
{
    uiBuilderList->clearSelection();
    uiBuilderList->setCurrentItem(item);

    QList<QListWidgetItem*> selectedItems = uiBuilderList->selectedItems();
    if (selectedItems.count() == 1 && selectedItems[0] == item) {
        accept();
        return;
    }

    assert(false);
}
