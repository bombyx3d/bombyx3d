#include "Shader.h"
#include "opengl.h"
#include "engine/core/Log.h"
#include "engine/interfaces/core/IThreadManager.h"
#include <utility>
#include <algorithm>
#include <iomanip>
#include <cassert>

namespace Engine
{
    Shader::Shader()
        : mProgramCompiled(false)
    {
        mVertexShader = glCreateShader(GL_VERTEX_SHADER);
        mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        mProgram = glCreateProgram();
        glAttachShader(GLuint(mProgram), GLuint(mVertexShader));
        glAttachShader(GLuint(mProgram), GLuint(mFragmentShader));
    }

    Shader::~Shader()
    {
        GLuint program = GLuint(mProgram);
        GLuint fragmentShader = GLuint(mFragmentShader);
        GLuint vertexShader = GLuint(mVertexShader);

        IThreadManager::instance()->performInRenderThread([program, fragmentShader, vertexShader]() {
            glDeleteProgram(program);
            glDeleteShader(fragmentShader);
            glDeleteShader(vertexShader);
        });
    }

    void Shader::setVertexSource(const std::vector<std::string>& source)
    {
        resetToUncompiledState();
        setSource(mVertexShader, source);
    }

    void Shader::setFragmentSource(const std::vector<std::string>& source)
    {
        resetToUncompiledState();
        setSource(mFragmentShader, source);
    }

    void Shader::use()
    {
        if (!mProgramCompiled)
            compile();
        glUseProgram(GLuint(mProgram));
    }

    #define Z_PRINT_INFO_LOG(STREAM, WHAT, HANDLE) \
        GLint bufferLength = 0; \
        glGet##WHAT##iv(GLuint(HANDLE), GL_INFO_LOG_LENGTH, &bufferLength); \
        if (bufferLength <= 0) { \
            (STREAM) << "Info log is not available."; \
        } else { \
            std::vector<char> buffer(static_cast<size_t>(bufferLength)); \
            GLsizei actualLength = 0; \
            glGet##WHAT##InfoLog(GLuint(HANDLE), GLsizei(bufferLength), &actualLength, buffer.data()); \
            \
            std::string infoLog(buffer.data(), size_t(actualLength)); \
            if (actualLength > 0 && infoLog[size_t(actualLength - 1)] == '\n') \
                infoLog.resize(size_t(actualLength - 1)); \
            \
            if (!infoLog.empty()) \
                (STREAM) << infoLog; \
            else \
                (STREAM) << "Info log is not available."; \
        }

    bool Shader::compile()
    {
        mProgramCompiled = true;

        if (!compileShader(mVertexShader, "vertex"))
            return false;
        if (!compileShader(mFragmentShader, "fragment"))
            return false;

        glLinkProgram(GLuint(mProgram));

        GLint status = GL_FALSE;
        glGetProgramiv(GLuint(mProgram), GL_LINK_STATUS, &status);

        if (status == GL_TRUE) {
            collectUniformsAndAttributes();
        } else {
            std::stringstream ss;
            ss << "Unable to link shader program.\n";
            ss << "===============\n";
            formatSource(mVertexShader, ss);
            ss << "---------------\n";
            formatSource(mFragmentShader, ss);
            ss << "---------------\n";
            Z_PRINT_INFO_LOG(ss, Program, mProgram);
            ss << "\n===============";
            Z_LOGE(ss.str());
        }

        return status == GL_TRUE;
    }

    bool Shader::compileShader(size_t shaderHandle, const char* shaderType)
    {
        glCompileShader(GLuint(shaderHandle));

        GLint status = GL_FALSE;
        glGetShaderiv(GLuint(shaderHandle), GL_COMPILE_STATUS, &status);

        if (status != GL_TRUE) {
            std::stringstream ss;
            ss << "Unable to compile " << shaderType << " shader.\n";
            ss << "===============\n";
            formatSource(shaderHandle, ss);
            ss << "---------------\n";
            Z_PRINT_INFO_LOG(ss, Shader, shaderHandle);
            ss << "\n===============";
            Z_LOGE(ss.str());
        }

        return status == GL_TRUE;
    }

    void Shader::setSource(size_t shaderHandle, const std::vector<std::string>& source)
    {
        std::vector<const GLchar*> lines;
        std::vector<GLint> lengths;

        size_t numLines = source.size();
        lines.reserve(numLines);
        lengths.reserve(numLines);

        for (const auto& line : source) {
            lines.push_back(line.c_str());
            lengths.push_back(GLint(line.length()));
        }

        glShaderSource(GLuint(shaderHandle), GLsizei(numLines), lines.data(), lengths.data());
    }

    void Shader::formatSource(size_t shaderHandle, std::stringstream& stream)
    {
        GLint bufferLength = 0;
        glGetShaderiv(GLuint(shaderHandle), GL_SHADER_SOURCE_LENGTH, &bufferLength);

        GLsizei sourceLength = 0;
        std::vector<char> shaderSource(static_cast<size_t>(bufferLength));
        glGetShaderSource(GLuint(shaderHandle), GLsizei(bufferLength), &sourceLength, shaderSource.data());

        const char* p = shaderSource.data();
        const char* end = p + sourceLength;
        bool printLineNumber = true;
        int lineNumber = 0;
        for (; p < end; ++p) {
            if (printLineNumber) {
                stream << std::setw(4) << ++lineNumber << std::setw(0) << ": ";
                printLineNumber = false;
            }

            stream << *p;
            if (*p == '\n')
                printLineNumber = true;
        }

        std::string source = stream.str();
        size_t length = source.length();
        if (length > 0 && source[length - 1] != '\n')
            stream << '\n';
    }

    void Shader::collectUniformsAndAttributes()
    {
        GLuint program = GLuint(mProgram);
        GLint size;
        GLenum type;

        GLint maxUniformNameLength = 0, maxAttributeNameLength = 0;
        glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
        glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeNameLength);

        size_t maxNameLength = size_t(std::max(maxAttributeNameLength, maxUniformNameLength));
        char* nameBuffer = reinterpret_cast<char*>(alloca(maxNameLength + 1));

        GLint numUniforms = 0, numAttributes = 0;
        glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
        glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);

        mUniforms.clear();
        for (size_t i = 0; i < size_t(numUniforms); i++) {
            GLsizei nameLength = 0;
            glGetActiveUniform(program, i, GLsizei(maxNameLength), &nameLength, &size, &type, nameBuffer);
            nameBuffer[nameLength] = 0;

            if (nameLength == 0)
                continue;
            if (nameLength >= 3 && nameBuffer[0] == 'g' && nameBuffer[1] == 'l' && nameBuffer[2] == '_')
                continue;

            int location = glGetUniformLocation(program, nameBuffer);
            assert(location >= 0);

            Atom atom = AtomTable::instance()->getAtom(std::string(nameBuffer, nameLength));
            mUniforms.emplace_back(atom, location);
        }

        mAttributes.clear();
        for (size_t i = 0; i < size_t(numAttributes); i++) {
            GLsizei nameLength = 0;
            glGetActiveAttrib(program, i, GLsizei(maxNameLength), &nameLength, &size, &type, nameBuffer);
            nameBuffer[nameLength] = 0;

            if (nameLength == 0)
                continue;
            if (nameLength >= 3 && nameBuffer[0] == 'g' && nameBuffer[1] == 'l' && nameBuffer[2] == '_')
                continue;

            int location = glGetAttribLocation(program, nameBuffer);
            assert(location >= 0);

            Atom atom = AtomTable::instance()->getAtom(std::string(nameBuffer, nameLength));
            mAttributes.emplace_back(atom, location);
        }
    }

    void Shader::resetToUncompiledState()
    {
        mProgramCompiled = false;
        mUniforms.clear();
        mAttributes.clear();
    }
}
