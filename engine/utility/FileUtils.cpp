#include "FileUtils.h"
#include "engine/interfaces/io/IFileSystem.h"
#include <sstream>

namespace Engine
{
    std::string FileUtils::makeFullPath(const std::string& fileName, const std::string& parentFileName)
    {
        size_t length = fileName.length();
        if (length > 0 && fileName[0] == '/')
            return fileName.substr(1);
        else {
            size_t index = parentFileName.rfind('/');
            std::string parentDir = (index == std::string::npos
                ? std::string()
                : parentFileName.substr(0, index + 1));
            return parentDir + fileName;
        }
    }

    std::string FileUtils::extractBaseName(const std::string& fileName)
    {
        size_t slashIndex = fileName.rfind('/');
        slashIndex = (slashIndex == std::string::npos ? 0 : slashIndex + 1);

        size_t dotIndex = fileName.rfind('.');
        dotIndex = (dotIndex == std::string::npos || dotIndex < slashIndex ? fileName.length() : dotIndex);

        return fileName.substr(slashIndex, dotIndex - slashIndex);
    }

    std::vector<char> FileUtils::loadFile(const std::string& fileName)
    {
        FilePtr file = IFileSystem::instance()->openFile(fileName);
        return loadFile(file.get());
    }

    std::vector<char> FileUtils::loadFile(IFile* file)
    {
        std::vector<char> result;

        if (!file)
            return result;

        size_t fileSize = size_t(file->size());
        result.resize(fileSize);

        size_t offset = 0;
        while (offset < fileSize) {
            size_t bytesRead = file->read(result.data() + offset, fileSize - offset);
            if (bytesRead == 0)
                break;
            offset += bytesRead;
        }

        if (offset != result.size())
            result.resize(offset);

        return result;
    }

    std::vector<char> FileUtils::loadFile(const FilePtr& file)
    {
        return loadFile(file.get());
    }

    std::vector<std::string> FileUtils::loadFileLines(const std::string& fileName, bool includeEolMarker)
    {
        FilePtr file = IFileSystem::instance()->openFile(fileName);
        return loadFileLines(file.get(), includeEolMarker);
    }

    std::vector<std::string> FileUtils::loadFileLines(IFile* file, bool includeEolMarker)
    {
        std::vector<char> fileData = loadFile(file);
        std::vector<std::string> result;

        for (auto it = fileData.begin(); it != fileData.end(); ) {
            std::stringstream ss;
            do {
                if (*it == '\n') {
                    if (includeEolMarker)
                        ss << *it;
                    ++it;
                    break;
                }
                ss << *it++;
            } while (it != fileData.end());

            std::string str = ss.str();
            size_t length = str.length();
            if (!includeEolMarker) {
                if (length > 0 && str[length - 1] == '\r')
                    str.resize(length - 1);
            } else {
                if (length > 1 && str[length - 2] == '\r' && str[length - 1] == '\n') {
                    str[length - 2] = '\n';
                    str.resize(length - 1);
                }
            }

            result.emplace_back(std::move(str));
        }

        return result;
    }

    std::vector<std::string> FileUtils::loadFileLines(const FilePtr& file, bool includeEolMarker)
    {
        return loadFileLines(file.get(), includeEolMarker);
    }
}
