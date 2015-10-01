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

#pragma once
#include "engine/interfaces/render/lowlevel/IShader.h"
#include "engine/core/Atom.h"
#include "engine/core/macros.h"
#include <sstream>

namespace B3D
{
    class GLES2Shader : public IShader
    {
    public:
        using UniformList = std::vector<std::pair<Atom, int>>;
        using AttributeList = std::vector<std::pair<Atom, int>>;

        GLES2Shader();
        ~GLES2Shader();

        size_t handle() const { return mProgram; }

        const UniformList& uniforms() const { return mUniforms; }
        const AttributeList& attributes() const { return mAttributes; }

        void setVertexSource(const std::vector<std::string>& source) override;
        void setFragmentSource(const std::vector<std::string>& source) override;

        void use();

        bool compile() override;

    private:
        size_t mVertexShader;
        size_t mFragmentShader;
        size_t mProgram;
        UniformList mUniforms;
        AttributeList mAttributes;
        bool mProgramCompiled;

        static bool compileShader(size_t shaderHandle, const char* shaderType);

        static void setSource(size_t shaderHandle, const std::vector<std::string>& source, bool fragment);
        static void formatSource(size_t shaderHandle, std::stringstream& stream);

        void collectUniformsAndAttributes();

        void resetToUncompiledState();

        B3D_DISABLE_COPY(GLES2Shader);
    };
}
