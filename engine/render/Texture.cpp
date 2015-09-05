#include "Texture.h"
#include "engine/interfaces/core/IThreadManager.h"
#include "opengl.h"

namespace Engine
{
    Texture::Texture()
    {
        GLuint handle = 0;
        glGenTextures(1, &handle);
        mHandle = handle;
    }

    Texture::~Texture()
    {
        GLuint handle = GLuint(mHandle);
        IThreadManager::instance()->performInRenderThread([handle]() {
            glDeleteTextures(1, &handle);
        });
    }
}
