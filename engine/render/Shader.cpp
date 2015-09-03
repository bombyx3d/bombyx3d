#include "Shader.h"
#include "opengl.h"

namespace Engine
{
    Shader::Shader()
    {
        mProgram = glCreateProgram();
    }

    Shader::~Shader()
    {
        glDeleteProgram(GLuint(mProgram));
    }
}
