#include "Texture.h"
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
        glDeleteTextures(1, &handle);
    }
}
