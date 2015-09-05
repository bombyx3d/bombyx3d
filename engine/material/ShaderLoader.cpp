#include "ShaderLoader.h"
#include "engine/utility/FileUtils.h"
#include "engine/interfaces/io/IFileSystem.h"
#include "engine/core/Log.h"
#include <cassert>

namespace Engine
{
    static const std::string VERTEX = "%vertex";
    static const std::string VERTEX_LF = "%vertex\n";
    static const std::string FRAGMENT = "%fragment";
    static const std::string FRAGMENT_LF = "%fragment\n";
    static const std::string COMMON = "%common";
    static const std::string COMMON_LF = "%common\n";
    static const std::string INCLUDE = "%include ";
    static const std::string LF = "\n";

    ShaderLoader::ShaderLoader()
    {
    }

    ShaderLoader::~ShaderLoader()
    {
    }

    bool ShaderLoader::loadFile(const std::string& fileName, std::vector<std::string>* what)
    {
        FilePtr file = IFileSystem::instance()->openFile(fileName);
        return loadFile(file.get(), what);
    }

    bool ShaderLoader::loadFile(IFile* file, std::vector<std::string>* what)
    {
        bool success = true;
        int lineNumber = 0;

        if (!file)
            return false;

        std::vector<std::string> lines = FileUtils::loadFileLines(file, true);
        for (auto& line : lines) {
            ++lineNumber;

            size_t index = line.find("//");
            if (index != std::string::npos) {
                bool endsWithLF = false;
                size_t length = line.length();
                if (length > 1 && line[length - 1] == '\n')
                    endsWithLF = true;

                line = line.substr(0, index);

                if (endsWithLF)
                    line += '\n';
            }

            if (*line.c_str() == '%') {
                if (line == VERTEX_LF || line == VERTEX)
                    what = &mVertex;
                else if (line == FRAGMENT_LF || line == FRAGMENT)
                    what = &mFragment;
                else if (line == COMMON_LF || line == COMMON)
                    what = nullptr;
                else if (line.substr(0, INCLUDE.length()) == INCLUDE) {
                    auto include = openIncludeFile(line.substr(INCLUDE.length()), file->name());
                    success = loadFile(include.get(), what) && success;
                } else {
                    Z_LOGE(file->name() << "(" << lineNumber << "): invalid directive.");
                    what = nullptr;
                    success = false;
                }
                continue;
            }

            if (what)
                what->emplace_back(std::move(line));
            else {
                mVertex.emplace_back(line);
                mFragment.emplace_back(std::move(line));
            }
        }

        if (!success)
            Z_LOGE("Unable to load shader \"" << file->name() << "\".");

        return success;
    }

    bool ShaderLoader::loadFile(const FilePtr& file, std::vector<std::string>* what)
    {
        return loadFile(file.get(), what);
    }

    FilePtr ShaderLoader::openIncludeFile(std::string fileName, const std::string& parentFileName) const
    {
        FilePtr file;

        size_t length = fileName.length();
        if (length > 0 && fileName[length - 1] == '\n')
            fileName.resize(--length);

        std::string name = FileUtils::makeFullPath(fileName, parentFileName);
        if (IFileSystem::instance()->fileExists(name))
            return IFileSystem::instance()->openFile(name);

        return IFileSystem::instance()->openFile(fileName);
    }
}
