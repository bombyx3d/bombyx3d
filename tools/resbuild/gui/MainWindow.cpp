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
    m_Project.reset();
}

void MainWindow::build(bool draft)
{
    if (!m_Project || m_BuildProgress)
        return;

    QDir targetPath = QFileInfo(m_FileName).absoluteDir();
    QString projectName = QFileInfo(m_FileName).completeBaseName();
    std::string outputFile = targetPath.absoluteFilePath(QString("%1.pak").arg(projectName)).toUtf8().constData();

    m_BuildProgress = new BuildProgressWidget(outputFile, m_Project, this);
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
    if (m_Project && m_ProjectModified) {
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
    m_ListWidgetItems.clear();
    m_ListWidgetRules.clear();

    m_Project.reset(new BuildProject(this));
    m_FileName = path;

    on_uiSaveFileButton_clicked();

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
    m_ListWidgetItems.clear();
    m_ListWidgetRules.clear();

    m_Project.reset(new BuildProject(this));
    m_FileName = path;

    m_LoadingProject = true;
    try {
        m_Project->load(m_FileName.toLocal8Bit().constData(), this);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, tr("Error"), tr("Unable to load file \"%1\": %2")
            .arg(m_FileName).arg(QString::fromUtf8(e.what())));
        m_Project.reset();
        uiRuleList->clear();
        m_ListWidgetItems.clear();
        m_ListWidgetRules.clear();
    }
    m_LoadingProject = false;

    uiRuleList->setCurrentItem(uiRuleList->count() > 0 ? uiRuleList->item(0) : nullptr);
    m_ProjectModified = false;

    updateUI();
}

bool MainWindow::on_uiSaveFileButton_clicked()
{
    if (!m_Project)
        return true;

    try {
        QDir targetPath = QFileInfo(m_FileName).absolutePath();
        m_Project->save(m_FileName.toLocal8Bit().constData(), [targetPath](const std::string& fileName) -> std::string {
            QString name = QString::fromUtf8(fileName.c_str());
            name = targetPath.relativeFilePath(name);
            return name.toUtf8().constData();
        });
    } catch (const std::exception& e) {
        updateUI();
        QMessageBox::critical(this, tr("Error"), tr("Unable to write file \"%1\": %2")
            .arg(m_FileName).arg(QString::fromUtf8(e.what())));
        return false;
    }

    m_ProjectModified = false;
    updateUI();

    return true;
}

void MainWindow::on_uiAddRuleButton_clicked()
{
    if (m_BuildProgress || !m_Project)
        return;

    NewRuleDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        std::string className = dialog.selectedClass();
        BuildRulePtr rule = BuildRule::createBuildRule(className, m_Project->nextRuleID(), this);
        rule->setName(tr("Rule %1").arg(rule->id()).toUtf8().constData());
        if (rule)
            m_Project->addRule(rule);
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

    for (int i = 0; i < selectedRules.count(); i++) {
        auto it = m_ListWidgetRules.find(selectedRules[i]);
        if (it != m_ListWidgetRules.end())
            m_Project->removeRule(it->second);
    }

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

void MainWindow::onProjectModified(BuildProject* project)
{
    if (m_Project.get() == project)
        m_ProjectModified = true;
    updateUI();
}

void MainWindow::onRuleCreated(BuildRule* rule)
{
    QListWidgetItem* item = new QListWidgetItem(QString::fromUtf8(rule->name().c_str()), uiRuleList);
    m_ListWidgetItems.insert({ rule, item });
    m_ListWidgetRules.insert({ item, rule });

    if (!m_BuildProgress && !m_LoadingProject)
        uiRuleList->setCurrentItem(item);

    updateUI();
}

void MainWindow::onRuleModified(BuildRule* rule)
{
    auto it = m_ListWidgetItems.find(rule);
    if (it != m_ListWidgetItems.end()) {
        auto item = it->second;
        item->setText(QString::fromUtf8(rule->name().c_str()));
        m_ProjectModified = true;
    }

    updateUI();
}

void MainWindow::onRuleDestroyed(BuildRule* rule)
{
    auto it = m_ListWidgetItems.find(rule);
    if (it != m_ListWidgetItems.end()) {
        m_ListWidgetRules.erase(it->second);
        delete it->second;
        m_ListWidgetItems.erase(it);
    }

    updateUI();
}

void MainWindow::updateUI()
{
    QList<QListWidgetItem*> selectedRules = uiRuleList->selectedItems();

    uiRuleList->setEnabled(m_Project != nullptr);
    uiSaveFileButton->setEnabled(m_Project != nullptr && m_ProjectModified);
    uiAddRuleButton->setEnabled(m_Project != nullptr);
    uiRemoveRuleButton->setEnabled(m_Project != nullptr && selectedRules.count() > 0);
    uiDraftBuildButton->setEnabled(m_Project != nullptr);
    uiFinalBuildButton->setEnabled(m_Project != nullptr);
    uiCleanButton->setEnabled(m_Project != nullptr);
}
