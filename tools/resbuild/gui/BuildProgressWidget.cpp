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
#include <exception>
#include <atomic>
#include <QFileInfo>
#include <QCloseEvent>
#include <QMessageBox>
#include <QThread>

class BuildProgressWidget::BuildThread : public QThread//, private BuildState
{
    Q_OBJECT

public:
    explicit BuildThread(const BuildProjectPtr& project, BuildProgressWidget* widget)
        : m_Widget(widget)
        , m_Project(project)
        , m_ShouldAbort(false)
    {
    }

    void run() override
    {
        // FIXME

        if (shouldAbort())
            emit buildAborted();
        else
            emit buildSucceeded();

        return;
    }

    bool shouldAbort() const /*final override*/ { return m_ShouldAbort.load(); }
    void abort() { m_ShouldAbort.store(true); }

signals:
    void buildSucceeded();
    void buildFailed();
    void buildAborted();

    void setStatus(const QString& message) /*final override*/;
    void setProgress(float progress);

    void printInfo(const QString& message) /*final override*/;
    void printWarning(const QString& message) /*final override*/;
    void printError(const QString& message) /*final override*/;

private:
    BuildProgressWidget* m_Widget;
    BuildProjectPtr m_Project;
    std::atomic<bool> m_ShouldAbort;
};


BuildProgressWidget::BuildProgressWidget(const BuildProjectPtr& project, QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);

    uiStatusLabel->setText(tr("Analyzing dependencies..."));
    uiProgressBar->setRange(0, 0);
    uiProgressBar->setValue(0);
    uiCloseButton->setVisible(false);
    uiLogView->setVisible(false);

    adjustSize();

    m_BuildThread = new BuildThread(project, this);
    connect(m_BuildThread, SIGNAL(buildSucceeded()), SLOT(buildSucceeded()));
    connect(m_BuildThread, SIGNAL(buildFailed()), SLOT(buildFailed()));
    connect(m_BuildThread, SIGNAL(buildAborted()), SLOT(buildAborted()));
    connect(m_BuildThread, SIGNAL(setStatus(const QString&)), SLOT(setStatusText(const QString&)));
    connect(m_BuildThread, SIGNAL(setProgress(float)), SLOT(setProgress(float)));
    connect(m_BuildThread, SIGNAL(printInfo(const QString&)), SLOT(printInfo(const QString&)));
    connect(m_BuildThread, SIGNAL(printWarning(const QString&)), SLOT(printWarning(const QString&)));
    connect(m_BuildThread, SIGNAL(printError(const QString&)), SLOT(printError(const QString&)));
    m_BuildThread->start();
}

BuildProgressWidget::~BuildProgressWidget()
{
    m_BuildThread->abort();
    m_BuildThread->wait();
    delete m_BuildThread;
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

void BuildProgressWidget::setProgress(float value)
{
    if (uiAbortButton->isVisible() && uiAbortButton->isEnabled()) {
        uiProgressBar->setRange(0, 100);
        uiProgressBar->setValue(int(value * 100.0f));
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
        m_BuildThread->abort();
    }
}

void BuildProgressWidget::on_uiCloseButton_clicked()
{
    close();
}

#include "BuildProgressWidget.moc"
