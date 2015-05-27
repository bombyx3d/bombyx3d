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
#include "ui_BuildProgressWidget.h"
#include "../core/BuildManager.h"
#include "../core/project/BuildProject.h"
#include <memory>
#include <QDialog>

class BuildProgressWidget : public QDialog, private Ui_BuildProgressWidget
{
    Q_OBJECT

public:
    BuildProgressWidget(const std::string& outputFile, const BuildProjectPtr& project,
        bool draft, QWidget* parent = nullptr);
    ~BuildProgressWidget();

signals:
    void buildWindowClosed();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    class Listener;

    std::unique_ptr<Listener> m_Listener;
    BuildManagerPtr m_BuildManager;
    bool m_CloseAfterAbort = false;
    bool m_HasWarnings = false;

    Q_SLOT void buildSucceeded();
    Q_SLOT void buildFailed();
    Q_SLOT void buildAborted();

    Q_SLOT void setStatusText(const QString& message);
    Q_SLOT void setProgress(double value, double maximum);

    Q_SLOT void printInfo(const QString& message);
    Q_SLOT void printWarning(const QString& message);
    Q_SLOT void printError(const QString& message);

    Q_SLOT void on_uiAbortButton_clicked();
    Q_SLOT void on_uiCloseButton_clicked();
};
