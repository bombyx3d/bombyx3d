
#pragma once
#include "engine/core/macros.h"
#include "engine/utility/PoolAllocator.h"
#include <glm/glm.hpp>

namespace Engine
{
    class Uniform
    {
    public:
        struct IUniformValue;

        Uniform();
        Uniform(Uniform&& other);
        ~Uniform();

        Uniform& operator=(Uniform&& other);

        void reset();

        void setFloat(float value);
        void setVec2(const glm::vec2& value);
        void setVec3(const glm::vec3& value);
        void setVec4(const glm::vec4& value);
        void setMat4(const glm::mat4& value);

        bool upload(int location);

    private:
        IUniformValue* mValue;
        int mBoundToLocation;

        Z_DISABLE_COPY(Uniform);
    };
}
