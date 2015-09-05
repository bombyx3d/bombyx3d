#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "opengl.h"

namespace Engine
{
    Renderer::Renderer()
    {
        glClearColor(0.7f, 0.3f, 0.1f, 1.0f);
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::setViewport(int x, int y, int w, int h)
    {
        glViewport(x, y, w, h);
    }

    void Renderer::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    ShaderPtr Renderer::createShader()
    {
        return std::make_shared<Shader>();
    }

    TexturePtr Renderer::createTexture()
    {
        return std::make_shared<Texture>();
    }

    void Renderer::useShader(const ShaderPtr& shader)
    {
        if (mCurrentShader != shader) {
            mCurrentShader = shader;
            if (!mCurrentShader)
                glUseProgram(0);
            else
                glUseProgram(GLuint(static_cast<Shader&>(*mCurrentShader).handle()));
        }
    }
}
