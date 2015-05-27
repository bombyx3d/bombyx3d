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
#include "BuildProgressWidget.h"
#include "../core/BuildManager.h"
#include <exception>
#include <atomic>
#include <QFileInfo>
#include <QCloseEvent>
#include <QMessageBox>
#include <QThread>

class BuildProgressWidget::Listener : public QObject, public BuildManager::Notifications
{
    Q_OBJECT

signals:
    void buildSucceeded() final override;
    void buildFailed() final override;
    void buildAborted() final override;

    void setStatus(const QString&);
    void setProgress(double current, double maximum) final override;

    void printVerbose(const QString&);
    void printInfo(const QString&);
    void printWarning(const QString&);
    void printError(const QString&);

private:
    void printVerbose(const std::string& message) final override
    {
        printVerbose(QString::fromUtf8(message.c_str()));
    }

    void printInfo(const std::string& message) final override
    {
        printInfo(QString::fromUtf8(message.c_str()));
    }

    void printWarning(const std::string& message) final override
    {
        printWarning(QString::fromUtf8(message.c_str()));
    }

    void printError(const std::string& message) final override
    {
        printError(QString::fromUtf8(message.c_str()));
    }

    void setStatus(BuildManager::Status status) final override
    {
        switch (status)
        {
        case BuildManager::AnalyzingDependencies: setStatus(tr("Analyzing dependencies...")); break;
        case BuildManager::CopyingUnmodifiedFiles: setStatus(tr("Copying unmodified files...")); break;
        case BuildManager::Finishing: setStatus(tr("Finishing...")); break;
        }
    }
};


BuildProgressWidget::BuildProgressWidget(const std::string& outputFile, const BuildProjectPtr& project,
        bool draft, QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);

    uiStatusLabel->setText(tr("Analyzing dependencies..."));
    uiProgressBar->setRange(0, 0);
    uiProgressBar->setValue(0);
    uiCloseButton->setVisible(false);
    uiLogView->setVisible(false);

    adjustSize();

    m_Listener.reset(new Listener);
    connect(m_Listener.get(), SIGNAL(buildSucceeded()), SLOT(buildSucceeded()), Qt::QueuedConnection);
    connect(m_Listener.get(), SIGNAL(buildFailed()), SLOT(buildFailed()), Qt::QueuedConnection);
    connect(m_Listener.get(), SIGNAL(buildAborted()), SLOT(buildAborted()), Qt::QueuedConnection);
    connect(m_Listener.get(), SIGNAL(setStatus(const QString&)), SLOT(setStatusText(const QString&)), Qt::QueuedConnection);
    connect(m_Listener.get(), SIGNAL(setProgress(double, double)), SLOT(setProgress(double, double)), Qt::QueuedConnection);
    connect(m_Listener.get(), SIGNAL(printInfo(const QString&)), SLOT(printInfo(const QString&)), Qt::QueuedConnection);
    connect(m_Listener.get(), SIGNAL(printWarning(const QString&)), SLOT(printWarning(const QString&)), Qt::QueuedConnection);
    connect(m_Listener.get(), SIGNAL(printError(const QString&)), SLOT(printError(const QString&)), Qt::QueuedConnection);

    auto mode = (draft ? BuildManager::INCREMENTAL_DRAFT_BUILD : BuildManager::FULL_OPTIMIZED_BUILD);
    m_BuildManager = std::make_shared<BuildManager>();
    m_BuildManager->startBuild(outputFile, project.get(), mode, m_Listener.get());
}

BuildProgressWidget::~BuildProgressWidget()
{
    m_BuildManager->requestAbort();
    m_BuildManager->wait();
    m_BuildManager.reset();
}

void BuildProgressWidget::closeEvent(QCloseEvent* event)
{
    if (uiCloseButton->isVisible() && uiCloseButton->isEnabled()) {
        event->accept();
        emit buildWindowClosed();
        return;
    }

    if (uiAbortButton->isVisible() && uiAbortButton->isEnabled()) {
        m_CloseAfterAbort = true;
        on_uiAbortButton_clicked();
    }

    event->ignore();
}

void BuildProgressWidget::buildSucceeded()
{
    uiAbortButton->setVisible(false);
    uiCloseButton->setVisible(true);
    uiProgressBar->setVisible(false);

    if (!m_HasWarnings) {
        uiStatusLabel->setText(tr("Build succeeded."));
        uiStatusLabel->setStyleSheet("padding: 2px; background-color: green; color: white");
    } else {
        uiStatusLabel->setText(tr("Build succeeded (with warnings)."));
        uiStatusLabel->setStyleSheet("padding: 2px; background-color: orange; color: white");
    }
}

void BuildProgressWidget::buildFailed()
{
    uiAbortButton->setVisible(false);
    uiCloseButton->setVisible(true);
    uiProgressBar->setVisible(false);

    uiStatusLabel->setText(tr("Build failed."));
    uiStatusLabel->setStyleSheet("padding: 2px; background-color: red; color: white");
}

void BuildProgressWidget::buildAborted()
{
    uiAbortButton->setVisible(false);
    uiCloseButton->setVisible(true);
    uiProgressBar->setVisible(false);

    uiStatusLabel->setText(tr("Build aborted."));
    uiStatusLabel->setStyleSheet("padding: 2px; background-color: red; color: white");
    
    if (m_CloseAfterAbort)
        close();
}

void BuildProgressWidget::setStatusText(const QString& message)
{
    if (uiAbortButton->isVisible() && uiAbortButton->isEnabled()) {
        uiStatusLabel->setText(message);
        uiLogView->appendHtml(QString("<p style=\"color: silver\">%1</p>").arg(message));
    }
}

void BuildProgressWidget::setProgress(double value, double maximum)
{
    if (uiAbortButton->isVisible() && uiAbortButton->isEnabled()) {
        if (maximum <= 0) {
            uiProgressBar->setRange(0, 0);
        } else {
            uiProgressBar->setRange(0, 100);
            uiProgressBar->setValue(int(value / maximum * 100));
        }
    }
}

void BuildProgressWidget::printInfo(const QString& message)
{
    uiLogView->appendHtml(QString("<p style=\"color: teal\">%1</p>").arg(message));
    uiLogView->setVisible(true);
}

void BuildProgressWidget::printWarning(const QString& message)
{
    uiLogView->appendHtml(QString("<p style=\"color: orange\">%1</p>").arg(message));
    uiLogView->setVisible(true);
    m_HasWarnings = true;
}

void BuildProgressWidget::printError(const QString& message)
{
    uiLogView->appendHtml(QString("<p style=\"color: red\">%1</p>").arg(message));
    uiLogView->setVisible(true);
    m_HasWarnings = true;
}

void BuildProgressWidget::on_uiAbortButton_clicked()
{
    int r = QMessageBox::question(this, tr("Confirmation"), tr("Do you wish to abort the build?"),
        QMessageBox::Abort | QMessageBox::No);

    if (r != QMessageBox::Abort)
        m_CloseAfterAbort = false;
    else {
        uiAbortButton->setEnabled(false);
        uiStatusLabel->setText(tr("Aborting..."));
        uiProgressBar->setRange(0, 0);
        m_BuildManager->requestAbort();
    }
}

void BuildProgressWidget::on_uiCloseButton_clicked()
{
    close();
}

#include "BuildProgressWidget.moc"
