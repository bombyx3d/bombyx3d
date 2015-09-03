
#pragma once
#include <cstdint>
#include <memory>
#include <string>

namespace Engine
{
    class IFile
    {
    public:
        virtual ~IFile() = default;

        virtual const std::string& name() const = 0;

        virtual uint64_t size() = 0;

        virtual uint64_t position() = 0;
        virtual bool seek(uint64_t pos) = 0;

        virtual size_t read(void* buffer, size_t bytes) = 0;
    };

    using FilePtr = std::shared_ptr<IFile>;
}
