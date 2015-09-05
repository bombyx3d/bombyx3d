
#pragma once
#include "engine/core/macros.h"
#include <glm/glm.hpp>

namespace Engine
{
    class Uniform
    {
    public:
        Uniform();
        ~Uniform();

    private:
        int mBoundToLocation;

        Z_DISABLE_COPY(Uniform);
    };
}
