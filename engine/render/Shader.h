
#pragma once
#include "engine/interfaces/render/IShader.h"
#include "engine/core/Atom.h"
#include "engine/core/macros.h"
#include <sstream>

namespace Engine
{
    class Shader : public IShader
    {
    public:
        using UniformList = std::vector<std::pair<Atom, int>>;
        using AttributeList = std::vector<std::pair<Atom, int>>;

        Shader();
        ~Shader();

        size_t handle() const { return mProgram; }

        const UniformList& uniforms() const { return mUniforms; }
        const AttributeList& attributes() const { return mAttributes; }

        void setVertexSource(const std::vector<std::string>& source) override;
        void setFragmentSource(const std::vector<std::string>& source) override;

        void use();

        bool compile();

    private:
        size_t mVertexShader;
        size_t mFragmentShader;
        size_t mProgram;
        UniformList mUniforms;
        AttributeList mAttributes;
        bool mProgramCompiled;

        static bool compileShader(size_t shaderHandle, const char* shaderType);

        static void setSource(size_t shaderHandle, const std::vector<std::string>& source);
        static void formatSource(size_t shaderHandle, std::stringstream& stream);

        void collectUniformsAndAttributes();

        void resetToUncompiledState();

        Z_DISABLE_COPY(Shader);
    };
}
