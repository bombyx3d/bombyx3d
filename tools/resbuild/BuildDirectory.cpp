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
#include "BuildDirectory.h"

BuildDirectory::BuildDirectory(QObject* parent)
    : QObject(parent)
    , m_Database(nullptr)
{
}

BuildDirectory::~BuildDirectory()
{
    if (m_Database)
        sqlite3_close_v2(m_Database);
}

bool BuildDirectory::init(const QDir& dir, const QString& projectName, QString* errorMessage)
{
    m_Dir = dir;

    if (m_Database) {
        sqlite3_close_v2(m_Database);
        m_Database = nullptr;
    }

    QString databaseFile = dir.absoluteFilePath(QString(".%1.dat").arg(projectName));
    int r = sqlite3_open_v2(databaseFile.toUtf8().constData(), &m_Database,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (r != SQLITE_OK) {
        if (errorMessage) {
            *errorMessage = tr("Unable to open database file \"%1\": %2")
                .arg(databaseFile).arg(QString::fromUtf8(sqlite3_errstr(r)));
        }
        return false;
    }

    char* errmsg = nullptr;
    r = sqlite3_exec(m_Database, "CREATE TABLE IF NOT EXISTS input_files ("
        "path TEXT PRIMARY KEY,"
        "lastModified INTEGER"
        ")", nullptr, nullptr, &errmsg);
    if (r != SQLITE_OK || errmsg) {
        if (errorMessage) {
            *errorMessage = tr("Unable to create table \"%1\" in the database: %2")
                .arg("input_files").arg(QString::fromUtf8(errmsg ? errmsg : "Unknown error."));
        }
        sqlite3_free(errmsg);
        return false;
    }

    errmsg = nullptr;
    r = sqlite3_exec(m_Database, "CREATE TABLE IF NOT EXISTS output_files ("
        "name TEXT PRIMARY KEY,"
        "size INTEGER,"
        "md4sum TEXT"
        ")", nullptr, nullptr, &errmsg);
    if (r != SQLITE_OK || errmsg) {
        if (errorMessage) {
            *errorMessage = tr("Unable to create table \"%1\" in the database: %2")
                .arg("output_files").arg(QString::fromUtf8(errmsg ? errmsg : "Unknown error."));
        }
        sqlite3_free(errmsg);
        return false;
    }

    return true;
}
