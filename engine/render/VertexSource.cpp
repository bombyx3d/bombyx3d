#include "VertexSource.h"
#include "opengl.h"
#include "engine/render/Buffer.h"
#include "engine/core/Log.h"
#include <cassert>

namespace Engine
{
    VertexSource::VertexSource()
    {
    }

    VertexSource::~VertexSource()
    {
    }

    void VertexSource::setAttribute(const Atom& name, VertexAttributeType type,
        const VertexBufferPtr& buffer, size_t offset, size_t stride)
    {
        assert(buffer != nullptr);
        if (buffer == nullptr)
            return;

        auto& attribute = mAttributes[name];
        attribute.buffer = std::static_pointer_cast<Buffer>(buffer);
        attribute.type = type;
        attribute.offset = offset;
        attribute.stride = stride;
    }

    void VertexSource::setIndexBuffer(const IndexBufferPtr& indexBuffer)
    {
        mIndexBuffer = std::static_pointer_cast<Buffer>(indexBuffer);
    }

    void VertexSource::bind(const Shader& shader)
    {
        if (!mEnabledArrays.empty())
            unbind();

        mEnabledArrays.reserve(shader.attributes().size());
        for (const auto& it : shader.attributes()) {
            auto jt = mAttributes.find(it.first);
            if (jt == mAttributes.end())
                Z_LOGW("Missing input for attribute \"" << it.first.text() << "\".");
            else {
                const auto& attr = jt->second;

                const void* offset = reinterpret_cast<void*>(attr.offset);
                int location = it.second;

                size_t count = 0;
                GLenum type = 0;
                switch (attr.type)
                {
                case VertexAttributeType::Float: type = GL_FLOAT; count = 1; break;
                case VertexAttributeType::Float2: type = GL_FLOAT; count = 2; break;
                case VertexAttributeType::Float3: type = GL_FLOAT; count = 3; break;
                case VertexAttributeType::Float4: type = GL_FLOAT; count = 4; break;
                }

                assert(count != 0 && type != 0);

                glBindBuffer(GL_ARRAY_BUFFER, attr.buffer->handle());
                glVertexAttribPointer(location, count, type, GL_FALSE, GLsizei(attr.stride), offset);
                glEnableVertexAttribArray(location);

                mEnabledArrays.push_back(location);
            }
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer ? mIndexBuffer->handle() : 0);
    }

    void VertexSource::unbind()
    {
        for (int location : mEnabledArrays)
            glDisableVertexAttribArray(location);
        mEnabledArrays.clear();
    }
}
