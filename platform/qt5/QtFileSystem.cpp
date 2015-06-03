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
#include "QtFileSystem.h"
#include "QtMacros.h"
#include "utility/debug.h"

#if QT_VERSION < 0x050400
 #define AppDataLocation DataLocation
#endif

namespace Z
{
    QtFileSystem::QtFileSystem(const QDir& baseDir)
        : m_BaseDir(baseDir)
    {
    }

    QtFileSystem::QtFileSystem(const QString& baseDir)
        : m_BaseDir(baseDir)
    {
    }

    QString QtFileSystem::absoluteFilePath(const std::string& file) const
    {
        return m_BaseDir.absoluteFilePath(QString::fromUtf8(file.c_str()));
    }

    QString QtFileSystem::getStandardPath(QStandardPaths::StandardLocation location)
    {
        QString path = QStandardPaths::writableLocation(location);
        if (path.length() == 0) {
            Z_LOG("Unable to determine path to standard writable location " << location << ".");
            return QString();
        }

        path = QDir(path).absolutePath();
        if (!QDir().mkpath(path)) {
            Z_LOG("Unable to create directory \"" << zqUtf8Printable(path) << "\".");
            return QString();
        }

        return path;
    }

    bool QtFileSystem::fileExists(const std::string& path)
    {
        return QFile(absoluteFilePath(path)).exists();
    }

    FileReaderPtr QtFileSystem::openFile(const std::string& path)
    {
        std::unique_ptr<QFile> file(new QFile(absoluteFilePath(path)));

        if (!file->open(QFile::ReadOnly)) {
            Z_LOG("Unable to open file \"" << zqUtf8Printable(file->fileName()) << "\": "
                << zqUtf8Printable(file->errorString()) << ".");
            return FileReaderPtr();
        }

        return std::make_shared<QtFileReader>(path, std::move(file));
    }
}
