#include "Shader.h"
#include "opengl.h"
#include "engine/core/Log.h"
#include <iomanip>

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
        glDeleteProgram(GLuint(mProgram));
        glDeleteShader(GLuint(mFragmentShader));
        glDeleteShader(GLuint(mVertexShader));
    }

    void Shader::setVertexSource(const std::vector<std::string>& source)
    {
        mProgramCompiled = false;
        setSource(mVertexShader, source);
    }

    void Shader::setFragmentSource(const std::vector<std::string>& source)
    {
        mProgramCompiled = false;
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

        if (status != GL_TRUE) {
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
}
