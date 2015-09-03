
#pragma once
#include "engine/interfaces/render/ITexture.h"
#include "engine/core/macros.h"

namespace Engine
{
    class Texture : public ITexture
    {
    public:
        Texture();
        ~Texture();

    private:
        size_t mHandle;

        Z_DISABLE_COPY(Texture);
    };
}
