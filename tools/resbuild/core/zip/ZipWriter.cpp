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
#include "ZipWriter.h"
#include "ZipReader.h"
#include "ZipExtraField.h"
#include <sstream>
#include <cstdint>
#include <stdexcept>
#include <zip.h>
#include <unzip.h>

ZipWriter::ZipWriter(BuildManager::Notifications* notifications)
    : m_Notifications(notifications)
{
}

ZipWriter::~ZipWriter()
{
    close();
}

bool ZipWriter::open(const std::string& fileName)
{
    m_FileName = fileName;
    m_ZipFile = zipOpen64(fileName.c_str(), APPEND_STATUS_ADDINZIP);
    return m_ZipFile != nullptr;
}

void ZipWriter::close()
{
    if (m_ZipFile) {
        zipClose(m_ZipFile, nullptr);
        m_ZipFile = nullptr;
    }
}

void ZipWriter::createFile(const std::string& entryName, uint64_t dosTime, const tm_zip* zipTime,
    const ZipExtraField* extraField, CompressionMethod compressionMethod, int compressionLevel)
{
    int method = -1;
    switch (compressionMethod)
    {
    case UNCOMPRESSED: method = 0; break;
    case DEFLATE: method = Z_DEFLATED; break;
    case BZIP2: method = Z_BZIP2ED; break;
    }

    zip_fileinfo fileInfo;
    memset(&fileInfo, 0, sizeof(fileInfo));
    if (zipTime)
        memcpy(&fileInfo.tmz_date, zipTime, sizeof(tm_zip));
    fileInfo.dosDate = uLong(dosTime);

    std::vector<char> extraData;
    if (extraField)
        extraField->toByteArray(extraData);

    int r = zipOpenNewFileInZip2_64(m_ZipFile, entryName.c_str(), &fileInfo, nullptr, 0, extraData.data(),
        static_cast<unsigned long>(extraData.size()), nullptr, method, compressionLevel, false, false);
    if (r != ZIP_OK) {
        std::stringstream ss;
        ss << "Unable to add entry \"" << entryName << "\" in ZIP file \"" << fileName() << "\" (code " << r << ").";
        throw std::runtime_error(ss.str());
    }

    m_EntryName = entryName;
}

void ZipWriter::writeFile(const void* data, size_t size)
{
    int r = zipWriteInFileInZip(m_ZipFile, data, static_cast<unsigned>(size));
    if (r <= 0) {
        std::stringstream ss;
        ss << "Unable to write entry \"" << m_EntryName << "\" to ZIP file \"" << fileName()
            << "\" (code " << r << ").";
        throw std::runtime_error(ss.str());
    }
}

void ZipWriter::closeFile()
{
    int r = zipCloseFileInZip(m_ZipFile);
    if (r != ZIP_OK) {
        std::stringstream ss;
        ss << "Unable to write metadata for entry \"" << m_EntryName << "\" in ZIP file \"" << fileName()
            << "\" (code " << r << ").";
        throw std::runtime_error(ss.str());
    }
}

bool ZipWriter::addCurrentFileFromZip(ZipReader* reader)
{
    char entryName[4096];

    unz_file_info64 unzFileInfo;
    int r = unzGetCurrentFileInfo64(reader->m_ZipFile, &unzFileInfo, entryName, sizeof(entryName), nullptr, 0, nullptr, 0);
    if (r != UNZ_OK) {
        std::stringstream ss;
        ss << "Unable to retrieve information for current entry in ZIP file \"" << reader->fileName()
            << "\" (code " << r << ").";
        m_Notifications->printWarning(ss.str());
        return false;
    }

    std::vector<char> globalExtraField;
    globalExtraField.resize(unzFileInfo.size_file_extra);
    std::vector<char> comment;
    comment.resize(unzFileInfo.size_file_comment);

    r = unzGetCurrentFileInfo64(reader->m_ZipFile, &unzFileInfo, entryName, sizeof(entryName),
        globalExtraField.data(), unzFileInfo.size_file_extra, comment.data(), unzFileInfo.size_file_comment);
    if (r != UNZ_OK) {
        std::stringstream ss;
        ss << "Unable to retrieve information for entry \"" << entryName << "\" in ZIP file \"" << reader->fileName()
            << "\" (code " << r << ").";
        m_Notifications->printWarning(ss.str());
        return false;
    }

    int method = 0, level = 0;
    r = unzOpenCurrentFile2(reader->m_ZipFile, &method, &level, true);
    if (r != UNZ_OK) {
        std::stringstream ss;
        ss << "Unable to access entry \"" << entryName << "\" in ZIP file \"" << reader->fileName()
            << "\" (code " << r << ").";
        m_Notifications->printWarning(ss.str());
        return false;
    }

    r = unzGetLocalExtrafield(reader->m_ZipFile, nullptr, 0);
    if (r < 0) {
        std::stringstream ss;
        ss << "Unable to retrieve local information for entry \"" << entryName << "\" in ZIP file \""
            << reader->fileName() << "\" (code " << r << ").";
        m_Notifications->printWarning(ss.str());
        return false;
    }

    std::vector<char> localExtraField;
    localExtraField.resize(size_t(r));

    r = unzGetLocalExtrafield(reader->m_ZipFile, localExtraField.data(), static_cast<unsigned>(localExtraField.size()));
    if (r < 0) {
        std::stringstream ss;
        ss << "Unable to retrieve local information for entry \"" << entryName << "\" in ZIP file \""
            << reader->fileName() << "\" (code " << r << ").";
        m_Notifications->printWarning(ss.str());
        return false;
    }

    zip_fileinfo zipFileInfo;
    memset(&zipFileInfo, 0, sizeof(zipFileInfo));
    memcpy(&zipFileInfo.tmz_date, &unzFileInfo.tmu_date, sizeof(tm_unz));
    zipFileInfo.dosDate = unzFileInfo.dosDate;
    zipFileInfo.internal_fa = unzFileInfo.internal_fa;
    zipFileInfo.external_fa = unzFileInfo.external_fa;

    r = zipOpenNewFileInZip2_64(m_ZipFile, entryName, &zipFileInfo, localExtraField.data(),
        static_cast<unsigned>(localExtraField.size()), globalExtraField.data(), unzFileInfo.size_file_extra,
        comment.data(), method, level, true, false);
    if (r != UNZ_OK) {
        std::stringstream ss;
        ss << "Unable to create entry \"" << entryName << "\" in ZIP file \""
            << fileName() << "\" (code " << r << ").";
        throw std::runtime_error(ss.str());
    }

    const size_t BUFFER_SIZE = 1048576;
    std::vector<char> buffer(BUFFER_SIZE);

    uint64_t bytesRemaining = unzFileInfo.compressed_size;
    while (bytesRemaining > 0) {
        size_t bytesToRead = (bytesRemaining > uint64_t(BUFFER_SIZE) ? BUFFER_SIZE : size_t(bytesRemaining));
        int bytesRead = unzReadCurrentFile(reader->m_ZipFile, buffer.data(), static_cast<unsigned>(bytesToRead));
        if (bytesRead <= 0) {
            std::stringstream ss;
            ss << "Unable to read entry \"" << entryName << "\" from ZIP file \""  << reader->fileName()
                << "\" (code " << bytesRead << ").";
            throw std::runtime_error(ss.str());
        }

        int r = zipWriteInFileInZip(m_ZipFile, buffer.data(), static_cast<unsigned>(bytesRead));
        if (r != UNZ_OK) {
            std::stringstream ss;
            ss << "Unable to write entry \"" << entryName << "\" to ZIP file \""  << fileName()
                << "\" (code " << r << ").";
            throw std::runtime_error(ss.str());
        }
    }

    r = zipCloseFileInZipRaw64(m_ZipFile, unzFileInfo.uncompressed_size, unzFileInfo.crc);
    if (r != UNZ_OK) {
        std::stringstream ss;
        ss << "Unable to write entry \"" << entryName << "\" to ZIP file \""  << fileName()
            << "\" (code " << r << ").";
        throw std::runtime_error(ss.str());
    }

    r = unzCloseCurrentFile(reader->m_ZipFile);
    if (r == UNZ_CRCERROR) {
        std::stringstream ss;
        ss << "Unable to read entry \"" << entryName << "\" from ZIP file \""  << reader->fileName()
            << "\" (invalid CRC).";
        throw std::runtime_error(ss.str());
    } else if (r != UNZ_OK) {
        std::stringstream ss;
        ss << "Unable to read entry \"" << entryName << "\" from ZIP file \""  << reader->fileName()
            << "\" (code " << r << ").";
        throw std::runtime_error(ss.str());
    }

    return true;
}
