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
#include "FileUtil.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>

bool FileUtil::writeFile(const QString& fileName, const QByteArray& data, bool keepBackupFile, QString* errorMessage)
{
    QFileInfo fileInfo(fileName);
    QDir targetDirectory(fileInfo.path());
    QString targetFileName = fileInfo.fileName();

    QString tempFileName = QFileInfo(targetDirectory, ".~" + targetFileName).filePath();
    QFile file(tempFileName);
    if (!file.open(QIODevice::WriteOnly)) {
        if (errorMessage)
            *errorMessage = tr("Unable to create file \"%1\": %2").arg(tempFileName).arg(file.errorString());
        return false;
    }

    if (file.write(data) != data.length()) {
        if (errorMessage)
            *errorMessage = tr("Unable to write file \"%1\": %2").arg(tempFileName).arg(file.errorString());
        file.close();
        file.remove();
        return false;
    }

    if (!file.flush()) {
        if (errorMessage)
            *errorMessage = tr("Unable to write file \"%1\": %2").arg(tempFileName).arg(file.errorString());
        file.close();
        file.remove();
        return false;
    }

    file.close();

    QString backupFileName = QFileInfo(targetDirectory, targetFileName + '~').filePath();
    QFile originalFile(fileName);
    QFile backupFile(backupFileName);
    if (backupFile.exists())
        backupFile.remove();
    if (originalFile.exists()) {
        if (!originalFile.rename(backupFileName)) {
            *errorMessage = tr("Unable to rename file \"%1\" to \"%2\": %3")
                .arg(fileName).arg(backupFileName).arg(originalFile.errorString());
            file.remove();
            return false;
        }
    }

    if (!file.rename(fileName)) {
        if (errorMessage) {
            *errorMessage = tr("Unable to rename file \"%1\" to \"%2\": %3")
                .arg(tempFileName).arg(fileName).arg(file.errorString());
        }
        file.remove();
        return false;
    }

    if (!keepBackupFile)
        backupFile.remove();

    return true;
}
