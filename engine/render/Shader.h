
#pragma once
#include "engine/interfaces/render/IShader.h"
#include "engine/core/macros.h"
#include <sstream>

namespace Engine
{
    class Shader : public IShader
    {
    public:
        Shader();
        ~Shader();

        void setVertexSource(const std::vector<std::string>& source) override;
        void setFragmentSource(const std::vector<std::string>& source) override;

        void use();

        bool compile();

    private:
        size_t mVertexShader;
        size_t mFragmentShader;
        size_t mProgram;
        bool mProgramCompiled;

        static bool compileShader(size_t shaderHandle, const char* shaderType);

        static void setSource(size_t shaderHandle, const std::vector<std::string>& source);
        static void formatSource(size_t shaderHandle, std::stringstream& stream);

        Z_DISABLE_COPY(Shader);
    };
}
