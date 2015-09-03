
#pragma once
#include "IFile.h"
#include "engine/utility/Singleton.h"
#include <memory>
#include <string>

namespace Engine
{
    class IFileSystem : public Singleton<IFileSystem>
    {
    public:
        virtual ~IFileSystem() = default;

        virtual bool fileExists(const std::string& name) = 0;
        virtual FilePtr openFile(const std::string& name) = 0;
    };

    using FileSystemPtr = std::shared_ptr<IFileSystem>;
}
