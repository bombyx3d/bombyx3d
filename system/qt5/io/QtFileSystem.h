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
#include "core/interfaces/IFileSystem.h"
#include "system/qt5/io/QtFileReader.h"
#include <QDir>
#include <QStandardPaths>

namespace Engine
{
    /** A Qt-based implementation of @ref IFileSystem. */
    class QtFileSystem : public IFileSystem
    {
    public:
        Z_IMPLEMENTATION(QtFileSystem)

        /**
         * Constructor.
         * @param baseDir Base directory for relative file paths.
         */
        explicit QtFileSystem(const QDir& baseDir);

        /**
         * Constructor.
         * @param baseDir Base directory for relative file paths.
         */
        explicit QtFileSystem(const QString& baseDir);

        /**
         * Retrieves an absolute path for the given relative path.
         * @param file Relative path to the file.
         * @return Absolute path to the file.
         */
        QString absoluteFilePath(const std::string& file) const;

        /**
         * Retrieves path to one of standard system directories.
         * This method also ensures that directory exists.
         * @return Absolute path to the directory or empty string in case of error.
         */
        static QString getStandardPath(QStandardPaths::StandardLocation location);

        bool fileExists(const std::string& path) override;
        Ptr<IFileReader> openFile(const std::string& path) override;

    private:
        QDir m_BaseDir;
    };
}
