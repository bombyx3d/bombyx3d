
#pragma once
#include "engine/core/macros.h"
#include "engine/interfaces/io/IFile.h"
#include <vector>
#include <string>

namespace Engine
{
    class ShaderLoader
    {
    public:
        ShaderLoader();
        ~ShaderLoader();

        const std::vector<std::string>& vertexSource() const { return mVertex; }
        const std::vector<std::string>& fragmentSource() const { return mFragment; }

        bool loadFile(const std::string& fileName, std::vector<std::string>* what = nullptr);
        bool loadFile(IFile* file, std::vector<std::string>* what = nullptr);
        bool loadFile(const FilePtr& file, std::vector<std::string>* what = nullptr);

    private:
        std::vector<std::string> mVertex;
        std::vector<std::string> mFragment;

        FilePtr openIncludeFile(std::string fileName, const std::string& parentFileName) const;

        Z_DISABLE_COPY(ShaderLoader);
    };
}
