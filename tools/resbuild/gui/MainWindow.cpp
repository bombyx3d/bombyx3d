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
#include "MainWindow.h"
#include "BuildProgressWidget.h"
#include "NewRuleDialog.h"
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);
    updateUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::build(bool draft)
{
    if (!m_Project || m_BuildProgress)
        return;

    QDir targetPath = QFileInfo(m_FileName).absoluteDir();
    QString projectName = QFileInfo(m_FileName).fileName();

    /*
    BuildDirectoryPtr buildDirectory = std::make_shared<BuildDirectory>();
    QString errorMessage = tr("Unknown error.");
    if (!buildDirectory->init(targetPath, projectName, &errorMessage)) {
        QMessageBox::critical(this, tr("Error"), errorMessage);
        return;
    }
    */

    m_BuildProgress = new BuildProgressWidget(m_Project, this);
    connect(m_BuildProgress, SIGNAL(buildWindowClosed()), this, SLOT(buildWindowClosed()));
    m_BuildProgress->setModal(true);
    m_BuildProgress->setWindowModality(Qt::ApplicationModal);
    m_BuildProgress->show();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (m_BuildProgress || !saveIfNeeded())
        event->ignore();
    else
        event->accept();
}

void MainWindow::buildWindowClosed()
{
    m_BuildProgress->hide();
    m_BuildProgress->deleteLater();
    m_BuildProgress = nullptr;
}

bool MainWindow::saveIfNeeded()
{
    if (m_Project /*&& m_Project->isModified()*/) {
        int r = QMessageBox::question(this, tr("Confirmation"), tr("Current file has been modified. Save?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (r == QMessageBox::Save)
            return on_uiSaveFileButton_clicked();
        else
            return r == QMessageBox::Discard;
    }
    return true;
}

void MainWindow::on_uiNewFileButton_clicked()
{
    if (m_BuildProgress || !saveIfNeeded())
        return;

    QString path = QFileDialog::getSaveFileName(this, tr("Create project"), m_FileName, tr("Project file (*.resproj)"));
    if (path.length() == 0)
        return;

    uiRuleList->clear();

    m_Project.reset(new BuildProject);
    /*
    connect(m_Project.get(), SIGNAL(updateUI()), SLOT(updateUI()));
    connect(m_Project.get(), SIGNAL(ruleCreated(Rule*)), SLOT(onRuleCreated(Rule*)));
    connect(m_Project.get(), SIGNAL(ruleDeleted(Rule*)), SLOT(onRuleDeleted(Rule*)));
    */
    m_FileName = path;

    /*
    QString message = tr("Unknown error.");
    if (!m_Project->save(m_FileName, &message)) {
        QMessageBox::critical(this, tr("Error"), tr("Unable to create file \"%1\": %2").arg(m_FileName).arg(message));
        m_Project.reset();
        uiRuleList->clear();
    }
    */

    updateUI();
}

void MainWindow::on_uiOpenFileButton_clicked()
{
    if (m_BuildProgress || !saveIfNeeded())
        return;

    QString path = QFileDialog::getOpenFileName(this, tr("Open project"), m_FileName, tr("Project file (*.resproj)"));
    if (path.length() == 0)
        return;

    uiRuleList->clear();

    /*
    m_Project.reset(new Project);
    connect(m_Project.get(), SIGNAL(updateUI()), SLOT(updateUI()));
    connect(m_Project.get(), SIGNAL(ruleCreated(Rule*)), SLOT(onRuleCreated(Rule*)));
    connect(m_Project.get(), SIGNAL(ruleDeleted(Rule*)), SLOT(onRuleDeleted(Rule*)));
    m_FileName = path;

    m_LoadingProject = true;
    QString message = tr("Unknown error.");
    bool result = m_Project->load(m_FileName, &message);
    m_LoadingProject = false;

    if (!result) {
        QMessageBox::critical(this, tr("Error"), tr("Unable to load file \"%1\": %2").arg(m_FileName).arg(message));
        m_Project.reset();
        uiRuleList->clear();
    }
    */

    uiRuleList->setCurrentItem(uiRuleList->count() > 0 ? uiRuleList->item(0) : nullptr);

    updateUI();
}

bool MainWindow::on_uiSaveFileButton_clicked()
{
    if (!m_Project)
        return true;

    /*
    QString message = tr("Unknown error.");
    if (!m_Project->save(m_FileName, &message)) {
        updateUI();
        QMessageBox::critical(this, tr("Error"), tr("Unable to write file \"%1\": %2").arg(m_FileName).arg(message));
        return false;
    }
    */

    updateUI();

    return true;
}

void MainWindow::on_uiAddRuleButton_clicked()
{
    if (m_BuildProgress || !m_Project)
        return;

    NewRuleDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        /*
        Builder::FactoryPtr factory = dialog.selectedFactory();
        if (factory)
            m_Project->createRule(factory->createBuilder());
        */
    }

    updateUI();
}

void MainWindow::on_uiRemoveRuleButton_clicked()
{
    if (m_BuildProgress || !m_Project)
        return;

    QList<QListWidgetItem*> selectedRules = uiRuleList->selectedItems();
    if (selectedRules.count() == 0)
        return;

    int r = QMessageBox::question(this, tr("Confirmation"),
        tr("Do you wish to remove selected rule from the project?\n\nThis action can't be undone!"),
        QMessageBox::Yes | QMessageBox::No);
    if (r == QMessageBox::No)
        return;

    /*
    for (int i = 0; i < selectedRules.count(); i++)
        m_Project->removeRule(selectedRules[i]);
    */

    updateUI();
}

void MainWindow::on_uiDraftBuildButton_clicked()
{
    build(true);
}

void MainWindow::on_uiFinalBuildButton_clicked()
{
    build(false);
}

void MainWindow::on_uiCleanButton_clicked()
{
    if (!m_Project)
        return;

    // FIXME
}

void MainWindow::on_uiRuleList_itemSelectionChanged()
{
    if (m_CurrentEditor) {
        m_CurrentEditor->deleteLater();
        m_CurrentEditor = nullptr;
    }

    uiContentsArea->setWidget(nullptr);

    if (!m_BuildProgress && !m_LoadingProject) {
        QList<QListWidgetItem*> selectedRules = uiRuleList->selectedItems();
        if (selectedRules.count() == 1) {
            /*
            Rule* rule = m_Project->ruleForItem(selectedRules[0]);
            if (rule) {
                m_CurrentEditor = rule->createEditor(uiContentsArea);
                if (m_CurrentEditor) {
                    uiContentsArea->setWidget(m_CurrentEditor);
                    if (rule->isNewlyCreatedRule) {
                        rule->isNewlyCreatedRule = false;
                        auto nameEdit = m_CurrentEditor->findChild<QLineEdit*>("nameEdit");
                        if (nameEdit) {
                            nameEdit->selectAll();
                            nameEdit->setFocus();
                        }
                    }
                }
            }
            */
        }
    }

    updateUI();
}

/*
void MainWindow::onRuleCreated(Rule* rule)
{
    if (!rule->listWidgetItem)
        rule->listWidgetItem = new QListWidgetItem(rule->builder()->icon(), rule->name(), uiRuleList);
    if (!m_BuildProgress && !m_LoadingProject)
        uiRuleList->setCurrentItem(rule->listWidgetItem);
}

void MainWindow::onRuleDeleted(Rule* rule)
{
    delete rule->listWidgetItem;
    rule->listWidgetItem = nullptr;
}
*/

void MainWindow::updateUI()
{
    QList<QListWidgetItem*> selectedRules = uiRuleList->selectedItems();

    uiRuleList->setEnabled(m_Project != nullptr);
    uiSaveFileButton->setEnabled(m_Project != nullptr/* && m_Project->isModified()*/);
    uiAddRuleButton->setEnabled(m_Project != nullptr);
    uiRemoveRuleButton->setEnabled(m_Project != nullptr && selectedRules.count() > 0);
    uiDraftBuildButton->setEnabled(m_Project != nullptr);
    uiFinalBuildButton->setEnabled(m_Project != nullptr);
    uiCleanButton->setEnabled(m_Project != nullptr);
}
