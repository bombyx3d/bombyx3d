
#pragma once
#include "engine/interfaces/io/IFile.h"
#include <cstdio>

namespace Engine
{
    class StdIoFile : public IFile
    {
    public:
        StdIoFile(FILE* handle, const std::string& path);
        ~StdIoFile();

        const std::string& name() const override;

        uint64_t size() override;

        uint64_t position() override;
        bool seek(uint64_t pos) override;

        size_t read(void* buffer, size_t bytes) override;

    private:
        FILE* mHandle;
        std::string mPath;

        Z_DISABLE_COPY(StdIoFile);
    };
}
