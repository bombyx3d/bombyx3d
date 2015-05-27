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
#include <cassert>
#include <QPushButton>

NewRuleDialog::NewRuleDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);
    on_uiBuilderList_itemSelectionChanged();

    /*
    for (const auto& factory : Builder::factories()) {
        auto item = new QListWidgetItem(factory->builderIcon, factory->builderName, uiBuilderList);
        m_Factories.emplace(std::make_pair(item, factory));
    }
    */
}

NewRuleDialog::~NewRuleDialog()
{
}

/*
Builder::FactoryPtr NewRuleDialog::selectedFactory() const
{
    QList<QListWidgetItem*> selectedItems = uiBuilderList->selectedItems();
    if (selectedItems.count() != 1)
        return nullptr;

    auto it = m_Factories.find(selectedItems[0]);
    return (it != m_Factories.end() ? it->second : Builder::FactoryPtr());
}
*/

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
