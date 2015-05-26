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
#include "ZipReader.h"
#include <vector>
#include <sstream>
#include <cstring>
#include <zip.h>
#include <unzip.h>

ZipReader::ZipReader(BuildManager::Notifications* notifications)
    : m_Notifications(notifications)
{
}

ZipReader::~ZipReader()
{
    close();
}

bool ZipReader::open(const std::string& fileName)
{
    m_FileName = fileName;

    m_ZipFile = unzOpen64(fileName.c_str());
    if (!m_ZipFile)
        return false;

    unzGoToFirstFile(m_ZipFile);
    return true;
}

void ZipReader::close()
{
    if (m_ZipFile) {
        unzClose(m_ZipFile);
        m_ZipFile = nullptr;
    }
}

bool ZipReader::seekToFile(const std::string& name)
{
    int r = unzLocateFile(m_ZipFile, name.c_str(), nullptr);
    return (r == UNZ_OK);
}

bool ZipReader::readExtraField(ZipExtraField* field)
{
    char entryName[4096];

    unz_file_info64 unzFileInfo;
    int r = unzGetCurrentFileInfo64(m_ZipFile, &unzFileInfo, entryName, sizeof(entryName),
        nullptr, 0, nullptr, 0);
    if (r != UNZ_OK) {
        std::stringstream ss;
        ss << "Unable to retrieve information for current entry in ZIP file \"" << fileName()
            << "\" (code " << r << ").";
        m_Notifications->printWarning(ss.str());
        return false;
    }

    r = unzGetLocalExtrafield(m_ZipFile, nullptr, 0);
    if (r < 0) {
        std::stringstream ss;
        ss << "Unable to retrieve local information for entry \"" << entryName << "\" in ZIP file \""
            << fileName() << "\" (code " << r << ").";
        m_Notifications->printWarning(ss.str());
        return false;
    }

    std::vector<char> localExtraField;
    localExtraField.resize(size_t(r));

    r = unzGetLocalExtrafield(m_ZipFile, localExtraField.data(), static_cast<unsigned>(localExtraField.size()));
    if (r < 0) {
        std::stringstream ss;
        ss << "Unable to retrieve local information for entry \"" << entryName << "\" in ZIP file \""
            << fileName() << "\" (code " << r << ").";
        m_Notifications->printWarning(ss.str());
        return false;
    }

    if (!field->fromByteArray(localExtraField)) {
        std::stringstream ss;
        ss << "Unable to decode local information for entry \"" << entryName << "\" in ZIP file \""
            << fileName() << "\" (code " << r << ").";
        m_Notifications->printWarning(ss.str());
        return false;
    }

    return true;
}
