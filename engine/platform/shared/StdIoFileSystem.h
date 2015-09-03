
#pragma once
#include "engine/interfaces/io/IFileSystem.h"

namespace Engine
{
    class StdIoFileSystem : public IFileSystem
    {
    public:
        explicit StdIoFileSystem(const std::string& basePath);
        ~StdIoFileSystem();

        bool fileExists(const std::string& name) override;
        FilePtr openFile(const std::string& name) override;

    private:
        std::string mBasePath;

        Z_DISABLE_COPY(StdIoFileSystem);
    };
}
