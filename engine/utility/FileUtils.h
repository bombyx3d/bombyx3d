
#pragma once
#include "engine/core/macros.h"
#include "engine/interfaces/io/IFile.h"
#include <vector>
#include <string>

namespace Engine
{
    namespace FileUtils
    {
        std::string makeFullPath(const std::string& fileName, const std::string& parentFileName);
        std::string extractBaseName(const std::string& fileName);

        std::vector<char> loadFile(const std::string& fileName);
        std::vector<char> loadFile(IFile* file);
        std::vector<char> loadFile(const FilePtr& file) { return loadFile(file.get()); }

        std::vector<std::string> loadFileLines(const std::string& fileName, bool includeEolMarker = true);
        std::vector<std::string> loadFileLines(IFile* file, bool includeEolMarker = true);
        std::vector<std::string> loadFileLines(const FilePtr& file, bool includeEolMarker = true);
    }
}
