/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "GLES2Uniform.h"
#include "engine/render/gles2/GLES2Texture.h"
#include "engine/utility/PoolAllocator.h"
#include "opengl.h"
#include <cassert>

namespace B3D
{
    struct GLES2Uniform::IUniformValue
    {
        virtual ~IUniformValue() = default;
        virtual void upload(int location, int* textureCount) = 0;

        void* operator new(size_t size);
        void operator delete(void* ptr);
    };

    #define B3D_UNIFORM_VALUE(NAME, TYPE, UPLOAD) \
        struct NAME : public GLES2Uniform::IUniformValue \
        { \
            const TYPE value; \
            NAME(const TYPE& v) : value(v) {} \
            void upload(int location, int* textureCount) override { (void)textureCount; UPLOAD; } \
        }

    namespace
    {
        B3D_UNIFORM_VALUE(FloatValue, float, glUniform1f(location, value));
        B3D_UNIFORM_VALUE(Vec2Value, glm::vec2, glUniform2fv(location, 1, &value[0]));
        B3D_UNIFORM_VALUE(Vec3Value, glm::vec3, glUniform3fv(location, 1, &value[0]));
        B3D_UNIFORM_VALUE(Vec4Value, glm::vec4, glUniform4fv(location, 1, &value[0]));
        B3D_UNIFORM_VALUE(Mat4Value, glm::mat4, glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]));

        B3D_UNIFORM_VALUE(TextureValue, TexturePtr, {
            glActiveTexture(GLenum(GL_TEXTURE0 + *textureCount));
            glBindTexture(GL_TEXTURE_2D, GLuint(static_cast<GLES2Texture&>(*value).handle()));
            glUniform1i(location, *textureCount);
            ++*textureCount;
        });

        union ValueUnion
        {
            // Stupid Visual Studio 2013 does not support non-POD members in unions
            char floatValue[sizeof(FloatValue)];
            char vec2Value[sizeof(Vec2Value)];
            char vec3Value[sizeof(Vec3Value)];
            char vec4Value[sizeof(Vec4Value)];
            char mat4Value[sizeof(Mat4Value)];
            char textureValue[sizeof(TextureValue)];
        };

        static PoolAllocator<ValueUnion> gValueAllocator;
    }

    void* GLES2Uniform::IUniformValue::operator new(size_t size)
    {
        assert(size <= sizeof(ValueUnion));
        (void)size;
        return gValueAllocator.alloc();
    }

    void GLES2Uniform::IUniformValue::operator delete(void* ptr)
    {
        gValueAllocator.free(reinterpret_cast<ValueUnion*>(ptr));
    }


    GLES2Uniform::GLES2Uniform()
        : mValue(nullptr)
    {
    }

    GLES2Uniform::GLES2Uniform(GLES2Uniform&& other)
        : mValue(other.mValue)
    {
        other.mValue = nullptr;
    }

    GLES2Uniform::~GLES2Uniform()
    {
        reset();
    }

    GLES2Uniform& GLES2Uniform::operator=(GLES2Uniform&& other)
    {
        mValue = other.mValue;
        other.mValue = nullptr;
        return *this;
    }

    void GLES2Uniform::reset()
    {
        delete mValue;
        mValue = nullptr;
    }

    void GLES2Uniform::setFloat(float value)
    {
        reset();
        mValue = new FloatValue(value);
    }

    void GLES2Uniform::setVec2(const glm::vec2& value)
    {
        reset();
        mValue = new Vec2Value(value);
    }

    void GLES2Uniform::setVec3(const glm::vec3& value)
    {
        reset();
        mValue = new Vec3Value(value);
    }

    void GLES2Uniform::setVec4(const glm::vec4& value)
    {
        reset();
        mValue = new Vec4Value(value);
    }

    void GLES2Uniform::setMat4(const glm::mat4& value)
    {
        reset();
        mValue = new Mat4Value(value);
    }

    void GLES2Uniform::setTexture(const TexturePtr& texture)
    {
        reset();
        mValue = new TextureValue(texture);
    }

    bool GLES2Uniform::upload(int location, int* textureCount)
    {
        if (!mValue || location < 0)
            return false;
        mValue->upload(location, textureCount);
        return true;
    }
}
