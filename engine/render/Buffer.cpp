#include "Buffer.h"
#include "engine/interfaces/core/IThreadManager.h"
#include "opengl.h"
#include <cassert>

namespace Engine
{
    static GLenum usageToGL(BufferUsage usage)
    {
        switch (usage)
        {
        case BufferUsage::Static: return GL_STATIC_DRAW;
        case BufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
        case BufferUsage::Stream: return GL_STREAM_DRAW;
        }

        assert(false);
        return GL_STATIC_DRAW;
    }


    Buffer::Buffer(size_t target)
        : mSize(0)
        , mTarget(target)
        , mUsage(BufferUsage::Static)
    {
        GLuint handle = 0;
        glGenBuffers(1, &handle);
        mHandle = handle;
    }

    Buffer::~Buffer()
    {
        GLuint handle = GLuint(mHandle);
        IThreadManager::instance()->performInRenderThread([handle]() {
            glDeleteBuffers(1, &handle);
        });
    }

    size_t Buffer::currentSize() const
    {
        return mSize;
    }

    void Buffer::initEmpty(size_t size, BufferUsage usage)
    {
        glBindBuffer(GLenum(mTarget), GLuint(mHandle));
        glBufferData(GLenum(mTarget), GLsizeiptr(size), nullptr, usageToGL(usage));
        mSize = size;
    }

    void Buffer::setData(const void* data, size_t size, BufferUsage usage)
    {
        glBindBuffer(GLenum(mTarget), GLuint(mHandle));
        glBufferData(GLenum(mTarget), GLsizeiptr(size), data, usageToGL(usage));
        mSize = size;
    }
}
