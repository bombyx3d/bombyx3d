#include "Uniform.h"
#include "engine/utility/PoolAllocator.h"
#include "opengl.h"
#include <cassert>

namespace Engine
{
    struct Uniform::IUniformValue
    {
        virtual ~IUniformValue() = default;
        virtual void upload(int location) = 0;

        void* operator new(size_t size);
        void operator delete(void* ptr);
    };

    #define Z_UNIFORM_VALUE(NAME, TYPE, UPLOAD) \
        struct NAME : public Uniform::IUniformValue \
        { \
            const TYPE value; \
            NAME(const TYPE& v) : value(v) {} \
            void upload(int location) override { UPLOAD; } \
        }

    namespace
    {
        Z_UNIFORM_VALUE(FloatValue, float, glUniform1f(location, value));
        Z_UNIFORM_VALUE(Vec2Value, glm::vec2, glUniform2fv(location, 1, &value[0]));
        Z_UNIFORM_VALUE(Vec3Value, glm::vec3, glUniform3fv(location, 1, &value[0]));
        Z_UNIFORM_VALUE(Vec4Value, glm::vec4, glUniform4fv(location, 1, &value[0]));
        Z_UNIFORM_VALUE(Mat4Value, glm::mat4, glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]));

        union ValueUnion
        {
            // Stupid Visual Studio 2013 does not support non-POD members in unions
            char floatValue[sizeof(FloatValue)];
            char vec2Value[sizeof(Vec2Value)];
            char vec3Value[sizeof(Vec3Value)];
            char vec4Value[sizeof(Vec4Value)];
            char mat4Value[sizeof(Mat4Value)];
        };

        static PoolAllocator<ValueUnion> gValueAllocator;
    }

    void* Uniform::IUniformValue::operator new(size_t size)
    {
        assert(size <= sizeof(ValueUnion));
        return gValueAllocator.alloc();
    }

    void Uniform::IUniformValue::operator delete(void* ptr)
    {
        gValueAllocator.free(reinterpret_cast<ValueUnion*>(ptr));
    }


    Uniform::Uniform()
        : mValue(nullptr)
        , mBoundToLocation(-1)
    {
    }

    Uniform::Uniform(Uniform&& other)
        : mValue(other.mValue)
        , mBoundToLocation(other.mBoundToLocation)
    {
        other.mValue = nullptr;
        other.mBoundToLocation = -1;
    }

    Uniform::~Uniform()
    {
        reset();
    }

    Uniform& Uniform::operator=(Uniform&& other)
    {
        mValue = other.mValue;
        mBoundToLocation = other.mBoundToLocation;
        other.mValue = nullptr;
        other.mBoundToLocation = -1;
        return *this;
    }

    void Uniform::reset()
    {
        delete mValue;
        mValue = nullptr;
        mBoundToLocation = -1;
    }

    void Uniform::setFloat(float value)
    {
        reset();
        mValue = new FloatValue(value);
    }

    void Uniform::setVec2(const glm::vec2& value)
    {
        reset();
        mValue = new Vec2Value(value);
    }

    void Uniform::setVec3(const glm::vec3& value)
    {
        reset();
        mValue = new Vec3Value(value);
    }

    void Uniform::setVec4(const glm::vec4& value)
    {
        reset();
        mValue = new Vec4Value(value);
    }

    void Uniform::setMat4(const glm::mat4& value)
    {
        reset();
        mValue = new Mat4Value(value);
    }

    bool Uniform::upload(int location)
    {
        if (!mValue || location < 0)
            return false;

        if (mBoundToLocation != location) {
            mValue->upload(location);
            mBoundToLocation = location;
        }

        return true;
    }
}
