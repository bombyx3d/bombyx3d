
#pragma once
#include "engine/interfaces/render/IShader.h"
#include "engine/core/macros.h"

namespace Engine
{
    class Shader : public IShader
    {
    public:
        Shader();
        ~Shader();

    private:
        size_t mProgram;

        Z_DISABLE_COPY(Shader);
    };
}
