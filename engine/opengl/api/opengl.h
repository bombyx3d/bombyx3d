/*
 * Copyright (c) 2014 Nikolay Zapolnov (zapolnov@gmail.com).
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
#include <cstddef>
#include <cstdint>

#ifdef _WIN32
 #ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
 #endif
 #ifndef NOMINMAX
  #define NOMINMAX
 #endif
 #include <windows.h>
#endif

#ifdef FALSE
 #undef FALSE
#endif

#ifdef TRUE
 #undef TRUE
#endif

#ifdef NO_ERROR
 #undef NO_ERROR
#endif

namespace GL
{
    enum Enum
    {
        FALSE = 0,
        TRUE = 1,
        ZERO = 0,
        ONE = 1,
        NO_ERROR = 0,
        NONE = 0,
        POINTS = 0x0000,
        LINES = 0x0001,
        #define Z_GL_CONSTANT_(X, Y) X = Y,
        #include "constants.h"
        #include "constants_ext.h"
        #undef Z_GL_CONSTANT_
        BLEND_EQUATION = BLEND_EQUATION_RGB,
        COMPRESSED_RGB_S3TC_DXT1_ANGLE = COMPRESSED_RGB_S3TC_DXT1_EXT,
        COMPRESSED_RGBA_S3TC_DXT1_ANGLE = COMPRESSED_RGBA_S3TC_DXT1_EXT,
        COMPRESSED_RGBA_S3TC_DXT3_ANGLE = COMPRESSED_RGBA_S3TC_DXT3_EXT,
        COMPRESSED_RGBA_S3TC_DXT5_ANGLE = COMPRESSED_RGBA_S3TC_DXT5_EXT,
        COMPRESSED_RED_RGTC1_ARB = COMPRESSED_RED_RGTC1_EXT,
        COMPRESSED_SIGNED_RED_RGTC1_ARB = COMPRESSED_RED_RGTC1_EXT,
        COMPRESSED_RED_GREEN_RGTC2_ARB = COMPRESSED_RED_RGTC1_EXT,
        COMPRESSED_SIGNED_RED_GREEN_RGTC2_ARB = COMPRESSED_RED_RGTC1_EXT,
        COMPRESSED_LUMINANCE_LATC1_NV = COMPRESSED_LUMINANCE_LATC1_EXT,
        COMPRESSED_SIGNED_LUMINANCE_LATC1_NV = COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT,
        COMPRESSED_LUMINANCE_ALPHA_LATC2_NV = COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT,
        COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_NV = COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT,
    };

    typedef void Void;
    typedef char Char;
    typedef unsigned char Boolean;
    typedef unsigned int Bitfield;
    typedef int8_t Byte;
    typedef uint8_t UByte;
    typedef short Short;
    typedef int Int;
    typedef int Sizei;
    typedef unsigned short UShort;
    typedef unsigned int UInt;
    typedef int32_t Fixed;
    typedef float Float;
    typedef float Clampf;
    typedef ptrdiff_t Intptr;
    typedef ptrdiff_t Sizeiptr;
}

namespace GL3
{
    enum Enum
    {
        #define Z_GL_CONSTANT_(X, Y) X = Y,
        #include "constants3.h"
        #undef Z_GL_CONSTANT_
    };
}

enum GLExtension
{
    #define Z_GL_EXTENSION_(X) X,
    #include "extensions.h"
    #undef Z_GL_EXTENSION_
};

namespace gl
{
    void InitWrappers();

    void ActiveTexture(GL::Enum texture);
    void AttachShader(GL::UInt program, GL::UInt shader);
    void BindAttribLocation(GL::UInt program, GL::UInt index, const GL::Char* name);
    void BindBuffer(GL::Enum target, GL::UInt buffer);
    void BindFramebuffer(GL::Enum target, GL::UInt framebuffer);
    void BindRenderbuffer(GL::Enum target, GL::UInt renderbuffer);
    void BindTexture(GL::Enum target, GL::UInt texture);
    void BlendColor(GL::Clampf red, GL::Clampf green, GL::Clampf blue, GL::Clampf alpha);
    void BlendEquation(GL::Enum mode);
    void BlendEquationSeparate(GL::Enum modeRGB, GL::Enum modeAlpha);
    void BlendFunc(GL::Enum sfactor, GL::Enum dfactor);
    void BlendFuncSeparate(GL::Enum srcRGB, GL::Enum dstRGB, GL::Enum srcAlpha, GL::Enum dstAlpha);
    void BufferData(GL::Enum target, GL::Sizeiptr size, const void* data, GL::Enum usage);
    void BufferSubData(GL::Enum target, GL::Intptr offset, GL::Sizeiptr size, const void* data);
    GL::Enum CheckFramebufferStatus(GL::Enum target);
    void Clear(GL::Bitfield mask);
    void ClearColor(GL::Clampf red, GL::Clampf green, GL::Clampf blue, GL::Clampf alpha);
    void ClearDepthf(GL::Clampf depth);
    void ClearStencil(GL::Int s);
    void ColorMask(GL::Boolean red, GL::Boolean green, GL::Boolean blue, GL::Boolean alpha);
    void CompileShader(GL::UInt shader);
    void CompressedTexImage2D(GL::Enum target, GL::Int level, GL::Enum internalformat, GL::Sizei width,
        GL::Sizei height, GL::Int border, GL::Sizei imageSize, const void* data);
    void CompressedTexSubImage2D(GL::Enum target, GL::Int level, GL::Int xoffset, GL::Int yoffset,
        GL::Sizei width, GL::Sizei height, GL::Enum format, GL::Sizei imageSize, const void* data);
    void CopyTexImage2D(GL::Enum target, GL::Int level, GL::Enum intfmt, GL::Int x, GL::Int y, GL::Sizei w,
        GL::Sizei h, GL::Int border);
    void CopyTexSubImage2D(GL::Enum target, GL::Int level, GL::Int xoff, GL::Int yoff, GL::Int x, GL::Int y,
        GL::Sizei w, GL::Sizei h);
    GL::UInt CreateProgram();
    GL::UInt CreateShader(GL::Enum type);
    void CullFace(GL::Enum mode);
    void DeleteBuffers(GL::Sizei n, const GL::UInt* buffers);
    void DeleteFramebuffers(GL::Sizei n, const GL::UInt* framebuffers);
    void DeleteProgram(GL::UInt program);
    void DeleteRenderbuffers(GL::Sizei n, const GL::UInt* renderbuffers);
    void DeleteShader(GL::UInt shader);
    void DeleteTextures(GL::Sizei n, const GL::UInt* textures);
    void DepthFunc(GL::Enum func);
    void DepthMask(GL::Boolean flag);
    void DepthRangef(GL::Clampf zNear, GL::Clampf zFar);
    void DetachShader(GL::UInt program, GL::UInt shader);
    void Disable(GL::Enum cap);
    void DisableVertexAttribArray(GL::UInt index);
    void DrawArrays(GL::Enum mode, GL::Int first, GL::Sizei count);
    void DrawElements(GL::Enum mode, GL::Sizei count, GL::Enum type, const void* indices);
    void Enable(GL::Enum cap);
    void EnableVertexAttribArray(GL::UInt index);
    void Finish();
    void Flush();
    void FramebufferRenderbuffer(GL::Enum target, GL::Enum attachment, GL::Enum renderbuffertarget,
        GL::UInt renderbuffer);
    void FramebufferTexture2D(GL::Enum target, GL::Enum attachment, GL::Enum textarget, GL::UInt texture,
        GL::Int level);
    void FrontFace(GL::Enum mode);
    void GenBuffers(GL::Sizei n, GL::UInt* buffers);
    void GenerateMipmap(GL::Enum target);
    void GenFramebuffers(GL::Sizei n, GL::UInt* framebuffers);
    void GenRenderbuffers(GL::Sizei n, GL::UInt* renderbuffers);
    void GenTextures(GL::Sizei n, GL::UInt* textures);
    void GetActiveAttrib(GL::UInt prog, GL::UInt idx, GL::Sizei bufsz, GL::Sizei* len, GL::Int* sz,
        GL::Enum* type, GL::Char* name);
    void GetActiveUniform(GL::UInt prog, GL::UInt idx, GL::Sizei bufsz, GL::Sizei* len, GL::Int* sz,
        GL::Enum* type, GL::Char* name);
    void GetAttachedShaders(GL::UInt program, GL::Sizei maxcount, GL::Sizei* count, GL::UInt* shaders);
    int GetAttribLocation(GL::UInt program, const GL::Char* name);
    void GetBooleanv(GL::Enum pname, GL::Boolean* params);
    void GetBufferParameteriv(GL::Enum target, GL::Enum pname, GL::Int* params);
    GL::Enum GetError();
    void GetFloatv(GL::Enum pname, GL::Float* params);
    void GetFramebufferAttachmentParameteriv(GL::Enum target, GL::Enum attachment, GL::Enum pname, GL::Int* params);
    void GetIntegerv(GL::Enum pname, GL::Int* params);
    void GetProgramiv(GL::UInt program, GL::Enum pname, GL::Int* params);
    void GetProgramInfoLog(GL::UInt program, GL::Sizei bufsize, GL::Sizei* length, GL::Char* infolog);
    void GetRenderbufferParameteriv(GL::Enum target, GL::Enum pname, GL::Int* params);
    void GetShaderiv(GL::UInt shader, GL::Enum pname, GL::Int* params);
    void GetShaderInfoLog(GL::UInt shader, GL::Sizei bufsize, GL::Sizei* length, GL::Char* infolog);
    void GetShaderPrecisionFormat(GL::Enum shadertype, GL::Enum precisiontype, GL::Int* range, GL::Int* precision);
    void GetShaderSource(GL::UInt shader, GL::Sizei bufsize, GL::Sizei* length, GL::Char* source);
    const GL::Char* GetString(GL::Enum name);
    void GetTexParameterfv(GL::Enum target, GL::Enum pname, GL::Float* params);
    void GetTexParameteriv(GL::Enum target, GL::Enum pname, GL::Int* params);
    void GetUniformfv(GL::UInt program, GL::Int location, GL::Float* params);
    void GetUniformiv(GL::UInt program, GL::Int location, GL::Int* params);
    int GetUniformLocation(GL::UInt program, const GL::Char* name);
    void GetVertexAttribfv(GL::UInt index, GL::Enum pname, GL::Float* params);
    void GetVertexAttribiv(GL::UInt index, GL::Enum pname, GL::Int* params);
    void GetVertexAttribPointerv(GL::UInt index, GL::Enum pname, void** pointer);
    void Hint(GL::Enum target, GL::Enum mode);
    GL::Boolean IsBuffer(GL::UInt buffer);
    GL::Boolean IsEnabled(GL::Enum cap);
    GL::Boolean IsFramebuffer(GL::UInt framebuffer);
    GL::Boolean IsProgram(GL::UInt program);
    GL::Boolean IsRenderbuffer(GL::UInt renderbuffer);
    GL::Boolean IsShader(GL::UInt shader);
    GL::Boolean IsTexture(GL::UInt texture);
    void LineWidth(GL::Float width);
    void LinkProgram(GL::UInt program);
    void PixelStorei(GL::Enum pname, GL::Int param);
    void PolygonOffset(GL::Float factor, GL::Float units);
    void ReadPixels(GL::Int x, GL::Int y, GL::Sizei width, GL::Sizei height, GL::Enum format, GL::Enum type,
        void* pixels);
    void ReleaseShaderCompiler();
    void RenderbufferStorage(GL::Enum target, GL::Enum internalformat, GL::Sizei width, GL::Sizei height);
    void SampleCoverage(GL::Clampf value, GL::Boolean invert);
    void Scissor(GL::Int x, GL::Int y, GL::Sizei width, GL::Sizei height);
    void ShaderBinary(GL::Sizei n, const GL::UInt* shaders, GL::Enum binaryformat, const void* binary,
        GL::Sizei length);
    void ShaderSource(GL::UInt shader, GL::Sizei count, const GL::Char** string, const GL::Int* lenth);
    void StencilFunc(GL::Enum func, GL::Int ref, GL::UInt mask);
    void StencilFuncSeparate(GL::Enum face, GL::Enum func, GL::Int ref, GL::UInt mask);
    void StencilMask(GL::UInt mask);
    void StencilMaskSeparate(GL::Enum face, GL::UInt mask);
    void StencilOp(GL::Enum fail, GL::Enum zfail, GL::Enum zpass);
    void StencilOpSeparate(GL::Enum face, GL::Enum fail, GL::Enum zfail, GL::Enum zpass);
    void TexImage2D(GL::Enum target, GL::Int level, GL::Int internalformat, GL::Sizei width, GL::Sizei height,
        GL::Int border, GL::Enum format, GL::Enum type, const void* pixels);
    void TexParameterf(GL::Enum target, GL::Enum pname, GL::Float param);
    void TexParameterfv(GL::Enum target, GL::Enum pname, const GL::Float* params);
    void TexParameteri(GL::Enum target, GL::Enum pname, GL::Int param);
    void TexParameteriv(GL::Enum target, GL::Enum pname, const GL::Int* params);
    void TexSubImage2D(GL::Enum target, GL::Int level, GL::Int xoffset, GL::Int yoffset, GL::Sizei width,
        GL::Sizei height, GL::Enum format, GL::Enum type, const void* pixels);
    void Uniform1f(GL::Int location, GL::Float x);
    void Uniform1fv(GL::Int location, GL::Sizei count, const GL::Float* v);
    void Uniform1i(GL::Int location, GL::Int x);
    void Uniform1iv(GL::Int location, GL::Sizei count, const GL::Int* v);
    void Uniform2f(GL::Int location, GL::Float x, GL::Float y);
    void Uniform2fv(GL::Int location, GL::Sizei count, const GL::Float* v);
    void Uniform2i(GL::Int location, GL::Int x, GL::Int y);
    void Uniform2iv(GL::Int location, GL::Sizei count, const GL::Int* v);
    void Uniform3f(GL::Int location, GL::Float x, GL::Float y, GL::Float z);
    void Uniform3fv(GL::Int location, GL::Sizei count, const GL::Float* v);
    void Uniform3i(GL::Int location, GL::Int x, GL::Int y, GL::Int z);
    void Uniform3iv(GL::Int location, GL::Sizei count, const GL::Int* v);
    void Uniform4f(GL::Int location, GL::Float x, GL::Float y, GL::Float z, GL::Float w);
    void Uniform4fv(GL::Int location, GL::Sizei count, const GL::Float* v);
    void Uniform4i(GL::Int location, GL::Int x, GL::Int y, GL::Int z, GL::Int w);
    void Uniform4iv(GL::Int location, GL::Sizei count, const GL::Int* v);
    void UniformMatrix2fv(GL::Int location, GL::Sizei count, GL::Boolean transpose, const GL::Float* value);
    void UniformMatrix3fv(GL::Int location, GL::Sizei count, GL::Boolean transpose, const GL::Float* value);
    void UniformMatrix4fv(GL::Int location, GL::Sizei count, GL::Boolean transpose, const GL::Float* value);
    void UseProgram(GL::UInt program);
    void ValidateProgram(GL::UInt program);
    void VertexAttrib1f(GL::UInt indx, GL::Float x);
    void VertexAttrib1fv(GL::UInt indx, const GL::Float* values);
    void VertexAttrib2f(GL::UInt indx, GL::Float x, GL::Float y);
    void VertexAttrib2fv(GL::UInt indx, const GL::Float* values);
    void VertexAttrib3f(GL::UInt indx, GL::Float x, GL::Float y, GL::Float z);
    void VertexAttrib3fv(GL::UInt indx, const GL::Float* values);
    void VertexAttrib4f(GL::UInt indx, GL::Float x, GL::Float y, GL::Float z, GL::Float w);
    void VertexAttrib4fv(GL::UInt indx, const GL::Float* values);
    void VertexAttribPointer(GL::UInt indx, GL::Int size, GL::Enum type, GL::Boolean norm, GL::Sizei stride,
        const void* ptr);
    void Viewport(GL::Int x, GL::Int y, GL::Sizei width, GL::Sizei height);
}

namespace gl3
{
    void TexImage3D(GL::Enum target, GL::Int level, GL::Int internalformat, GL::Sizei width, GL::Sizei height,
        GL::Sizei depth, GL::Int border, GL::Enum format, GL::Enum type, const void *pixels);
    void CompressedTexImage3D(GL::Enum target, GL::Int level, GL::Enum internalformat, GL::Sizei width,
        GL::Sizei height, GL::Sizei depth, GL::Int border, GL::Sizei imageSize, const void *data);
}

namespace gl
{
    const char* GetVersionString();
    void GetVersionNumber(int* major, int* minor, int* release = nullptr, bool* es = nullptr);

    bool isOpenGLES(int major = 0, int minor = 0);
    bool isDesktopOpenGL(int major = 0, int minor = 0, int release = 0);
    bool Supports2DArrayTextures();
    bool Supports3DTextures();

    GL::Int GetInteger(GL::Enum param);
    void EnableOrDisable(GL::Enum cap, bool flag);
    bool IsExtensionSupported(const char* name);
    bool IsExtensionSupported(GLExtension extension);
}

namespace gl3
{
    void BindTexture(GL3::Enum target, GL::UInt texture);
    void GetIntegerv(GL3::Enum pname, GL::Int* params);
}
