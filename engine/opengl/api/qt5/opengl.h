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
#include "../opengl.h"
#include "../error.h"
#include <QGLContext>
#include <QGLFunctions>
#include <cassert>
#include <mutex>
#include <thread>

static QGLFunctions g_OpenGLFunctions;
static std::once_flag g_Init;

void gl::InitWrappers()
{
    std::call_once(g_Init, [](){
        g_OpenGLFunctions.initializeGLFunctions();
    });
}

void gl::ActiveTexture(GL::Enum texture)
{
    g_OpenGLFunctions.glActiveTexture(texture);
    Z_CHECK_GL_ERROR1(glActiveTexture, texture);
}

void gl::AttachShader(GL::UInt program, GL::UInt shader)
{
    g_OpenGLFunctions.glAttachShader(program, shader);
    Z_CHECK_GL_ERROR2(glAttachShader, program, shader);
}

void gl::BindAttribLocation(GL::UInt program, GL::UInt index, const GL::Char* name)
{
    g_OpenGLFunctions.glBindAttribLocation(program, index, name);
    Z_CHECK_GL_ERROR3(glBindAttribLocation, program, index, name);
}

void gl::BindBuffer(GL::Enum target, GL::UInt buffer)
{
    g_OpenGLFunctions.glBindBuffer(target, buffer);
    Z_CHECK_GL_ERROR2(glBindBuffer, target, buffer);
}

void gl::BindFramebuffer(GL::Enum target, GL::UInt framebuffer)
{
    g_OpenGLFunctions.glBindFramebuffer(target, framebuffer);
    Z_CHECK_GL_ERROR2(glBindFramebuffer, target, framebuffer);
}

void gl::BindRenderbuffer(GL::Enum target, GL::UInt renderbuffer)
{
    g_OpenGLFunctions.glBindRenderbuffer(target, renderbuffer);
    Z_CHECK_GL_ERROR2(glBindRenderbuffer, target, renderbuffer);
}

void gl::BindTexture(GL::Enum target, GL::UInt texture)
{
    glBindTexture(target, texture);
    Z_CHECK_GL_ERROR2(glBindTexture, target, texture);
}

void gl::BlendColor(GL::Clampf red, GL::Clampf green, GL::Clampf blue, GL::Clampf alpha)
{
    g_OpenGLFunctions.glBlendColor(red, green, blue, alpha);
    Z_CHECK_GL_ERROR4(glBlendColor, red, green, blue, alpha);
}

void gl::BlendEquation(GL::Enum mode)
{
    g_OpenGLFunctions.glBlendEquation(mode);
    Z_CHECK_GL_ERROR1(glBlendEquation, mode);
}

void gl::BlendEquationSeparate(GL::Enum modeRGB, GL::Enum modeAlpha)
{
    g_OpenGLFunctions.glBlendEquationSeparate(modeRGB, modeAlpha);
    Z_CHECK_GL_ERROR2(glBlendEquationSeparate, modeRGB, modeAlpha);
}

void gl::BlendFunc(GL::Enum sfactor, GL::Enum dfactor)
{
    glBlendFunc(sfactor, dfactor);
    Z_CHECK_GL_ERROR2(glBlendFunc, sfactor, dfactor);
}

void gl::BlendFuncSeparate(GL::Enum srcRGB, GL::Enum dstRGB, GL::Enum srcAlpha, GL::Enum dstAlpha)
{
    g_OpenGLFunctions.glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
    Z_CHECK_GL_ERROR4(glBlendFunc, srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void gl::BufferData(GL::Enum target, GL::Sizeiptr size, const void* data, GL::Enum usage)
{
    g_OpenGLFunctions.glBufferData(target, size, data, usage);
    Z_CHECK_GL_ERROR4(glBufferData, target, size, data, usage);
}

void gl::BufferSubData(GL::Enum target, GL::Intptr offset, GL::Sizeiptr size, const void* data)
{
    g_OpenGLFunctions.glBufferSubData(target, offset, size, data);
    Z_CHECK_GL_ERROR4(glBufferSubData, target, offset, size, data);
}

GL::Enum gl::CheckFramebufferStatus(GL::Enum target)
{
    GLenum status = g_OpenGLFunctions.glCheckFramebufferStatus(target);
    Z_CHECK_GL_ERROR1(glCheckFramebufferStatus, target);
    return static_cast<GL::Enum>(status);
}

void gl::Clear(GL::Bitfield mask)
{
    glClear(mask);
    Z_CHECK_GLCLEAR_ERROR(glClear, mask);
}

void gl::ClearColor(GL::Clampf red, GL::Clampf green, GL::Clampf blue, GL::Clampf alpha)
{
    glClearColor(red, green, blue, alpha);
    Z_CHECK_GL_ERROR4(glClearColor, red, green, blue, alpha);
}

void gl::ClearDepthf(GL::Clampf depth)
{
    g_OpenGLFunctions.glClearDepthf(depth);
    Z_CHECK_GL_ERROR1(glClearDepth, depth);
}

void gl::ClearStencil(GL::Int s)
{
    glClearStencil(s);
    Z_CHECK_GL_ERROR1(glClearStencil, s);
}

void gl::ColorMask(GL::Boolean red, GL::Boolean green, GL::Boolean blue, GL::Boolean alpha)
{
    glColorMask(red, green, blue, alpha);
    Z_CHECK_GL_ERROR4(glColorMask, red, green, blue, alpha);
}

void gl::CompileShader(GL::UInt shader)
{
    g_OpenGLFunctions.glCompileShader(shader);
    Z_CHECK_GL_ERROR1(glCompileShader, shader);
}

void gl::CompressedTexImage2D(GL::Enum target, GL::Int level, GL::Enum internalformat, GL::Sizei width,
    GL::Sizei height, GL::Int border, GL::Sizei imageSize, const void* data)
{
    g_OpenGLFunctions.glCompressedTexImage2D(target, level, internalformat, width, height, border,
        imageSize, data);
    Z_CHECK_GL_ERROR8(glCompressedTexImage2D, target, level, internalformat, width, height, border,
        imageSize, data);
}

void gl::CompressedTexSubImage2D(GL::Enum target, GL::Int level, GL::Int xoffset, GL::Int yoffset,
    GL::Sizei width, GL::Sizei height, GL::Enum format, GL::Sizei imageSize, const void* data)
{
    g_OpenGLFunctions.glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format,
        imageSize, data);
    Z_CHECK_GL_ERROR9(glCompressedTexSubImage2D, target, level, xoffset, yoffset, width, height, format,
        imageSize, data);
}

void gl::CopyTexImage2D(GL::Enum target, GL::Int level, GL::Enum intfmt, GL::Int x, GL::Int y, GL::Sizei w,
    GL::Sizei h, GL::Int border)
{
    glCopyTexImage2D(target, level, intfmt, x, y, w, h, border);
    Z_CHECK_GL_ERROR8(glCopyTexImage2D, target, level, intfmt, x, y, w, h, border);
}

void gl::CopyTexSubImage2D(GL::Enum target, GL::Int level, GL::Int xoff, GL::Int yoff, GL::Int x, GL::Int y,
    GL::Sizei w, GL::Sizei h)
{
    glCopyTexSubImage2D(target, level, xoff, yoff, x, y, w, h);
    Z_CHECK_GL_ERROR8(glCopyTexSubImage2D, target, level, xoff, yoff, x, y, w, h);
}

GL::UInt gl::CreateProgram()
{
    GL::UInt handle = g_OpenGLFunctions.glCreateProgram();
    Z_CHECK_GL_ERROR0(glCreateProgram);
    return handle;
}

GL::UInt gl::CreateShader(GL::Enum type)
{
    GL::UInt handle = g_OpenGLFunctions.glCreateShader(type);
    Z_CHECK_GL_ERROR1(glCreateShader, type);
    return handle;
}

void gl::CullFace(GL::Enum mode)
{
    glCullFace(mode);
    Z_CHECK_GL_ERROR1(glCullFace, mode);
}

void gl::DeleteBuffers(GL::Sizei n, const GL::UInt* buffers)
{
    g_OpenGLFunctions.glDeleteBuffers(n, buffers);
    Z_CHECK_GL_ERROR2(glDeleteBuffers, n, buffers);
}

void gl::DeleteFramebuffers(GL::Sizei n, const GL::UInt* framebuffers)
{
    g_OpenGLFunctions.glDeleteFramebuffers(n, framebuffers);
    Z_CHECK_GL_ERROR2(glDeleteFramebuffers, n, framebuffers);
}

void gl::DeleteProgram(GL::UInt program)
{
    g_OpenGLFunctions.glDeleteProgram(program);
    Z_CHECK_GL_ERROR1(glDeleteProgram, program);
}

void gl::DeleteRenderbuffers(GL::Sizei n, const GL::UInt* renderbuffers)
{
    g_OpenGLFunctions.glDeleteRenderbuffers(n, renderbuffers);
    Z_CHECK_GL_ERROR2(glDeleteRenderbuffers, n, renderbuffers);
}

void gl::DeleteShader(GL::UInt shader)
{
    g_OpenGLFunctions.glDeleteShader(shader);
    Z_CHECK_GL_ERROR1(glDeleteShader, shader);
}

void gl::DeleteTextures(GL::Sizei n, const GL::UInt* textures)
{
    glDeleteTextures(n, textures);
    Z_CHECK_GL_ERROR2(glDeleteTextures, n, textures);
}

void gl::DepthFunc(GL::Enum func)
{
    glDepthFunc(func);
    Z_CHECK_GL_ERROR1(glDepthFunc, func);
}

void gl::DepthMask(GL::Boolean flag)
{
    glDepthMask(flag);
    Z_CHECK_GL_ERROR1(glDepthMask, flag);
}

void gl::DepthRangef(GL::Clampf zNear, GL::Clampf zFar)
{
    g_OpenGLFunctions.glDepthRangef(zNear, zFar);
    Z_CHECK_GL_ERROR2(glDepthRangef, zNear, zFar);
}

void gl::DetachShader(GL::UInt program, GL::UInt shader)
{
    g_OpenGLFunctions.glDetachShader(program, shader);
    Z_CHECK_GL_ERROR2(glDetachShader, program, shader);
}

void gl::Disable(GL::Enum cap)
{
    glDisable(cap);
    Z_CHECK_GL_ERROR1(glDisable, cap);
}

void gl::DisableVertexAttribArray(GL::UInt index)
{
    g_OpenGLFunctions.glDisableVertexAttribArray(index);
    Z_CHECK_GL_ERROR1(glDisableVertexAttribArray, index);
}

void gl::DrawArrays(GL::Enum mode, GL::Int first, GL::Sizei count)
{
    glDrawArrays(mode, first, count);
    Z_CHECK_GL_ERROR3(glDrawArray, mode, first, count);
}

void gl::DrawElements(GL::Enum mode, GL::Sizei count, GL::Enum type, const void* indices)
{
    glDrawElements(mode, count, type, indices);
    Z_CHECK_GL_ERROR4(glDrawElements, mode, count, type, indices);
}

void gl::Enable(GL::Enum cap)
{
    glEnable(cap);
    Z_CHECK_GL_ERROR1(glEnable, cap);
}

void gl::EnableVertexAttribArray(GL::UInt index)
{
    g_OpenGLFunctions.glEnableVertexAttribArray(index);
    Z_CHECK_GL_ERROR1(glEnableVertexAttribArray, index);
}

void gl::Finish()
{
    glFinish();
    Z_CHECK_GL_ERROR0(glFinish);
}

void gl::Flush()
{
    glFlush();
    Z_CHECK_GL_ERROR0(glFlush);
}

void gl::FramebufferRenderbuffer(GL::Enum target, GL::Enum attachment, GL::Enum renderbuffertarget,
    GL::UInt renderbuffer)
{
    g_OpenGLFunctions.glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
    Z_CHECK_GL_ERROR4(glFramebufferRenderbuffer, target, attachment, renderbuffertarget, renderbuffer);
}

void gl::FramebufferTexture2D(GL::Enum target, GL::Enum attachment, GL::Enum textarget, GL::UInt texture,
    GL::Int level)
{
    g_OpenGLFunctions.glFramebufferTexture2D(target, attachment, textarget, texture, level);
    Z_CHECK_GL_ERROR5(glFramebufferTexture2D, target, attachment, textarget, texture, level);
}

void gl::FrontFace(GL::Enum mode)
{
    glFrontFace(mode);
    Z_CHECK_GL_ERROR1(glFrontFace, mode);
}

void gl::GenBuffers(GL::Sizei n, GL::UInt* buffers)
{
    g_OpenGLFunctions.glGenBuffers(n, buffers);
    Z_CHECK_GL_ERROR2(glGenBuffers, n, buffers);
}

void gl::GenerateMipmap(GL::Enum target)
{
    g_OpenGLFunctions.glGenerateMipmap(target);
    Z_CHECK_GL_ERROR1(glGenerateMipmap, target);
}

void gl::GenFramebuffers(GL::Sizei n, GL::UInt* framebuffers)
{
    g_OpenGLFunctions.glGenFramebuffers(n, framebuffers);
    Z_CHECK_GL_ERROR2(glGenFramebuffers, n, framebuffers);
}

void gl::GenRenderbuffers(GL::Sizei n, GL::UInt* renderbuffers)
{
    g_OpenGLFunctions.glGenRenderbuffers(n, renderbuffers);
    Z_CHECK_GL_ERROR2(glGenRenderbuffers, n, renderbuffers);
}

void gl::GenTextures(GL::Sizei n, GL::UInt* textures)
{
    glGenTextures(n, textures);
    Z_CHECK_GL_ERROR2(glGenTextures, n, textures);
}

void gl::GetActiveAttrib(GL::UInt prog, GL::UInt idx, GL::Sizei bufsz, GL::Sizei* len, GL::Int* sz,
    GL::Enum* type, GL::Char* name)
{
    GLenum tmp_type = 0;
    g_OpenGLFunctions.glGetActiveAttrib(prog, idx, bufsz, len, sz, &tmp_type, name);
    Z_CHECK_GL_ERROR7(glGetActiveAttrib, prog, idx, bufsz, len, sz, type, name);
    *type = static_cast<GL::Enum>(tmp_type);
}

void gl::GetActiveUniform(GL::UInt prog, GL::UInt idx, GL::Sizei bufsz, GL::Sizei* len, GL::Int* sz,
    GL::Enum* type, GL::Char* name)
{
    GLenum tmp_type = 0;
    g_OpenGLFunctions.glGetActiveUniform(prog, idx, bufsz, len, sz, &tmp_type, name);
    Z_CHECK_GL_ERROR7(glGetActiveAttrib, prog, idx, bufsz, len, sz, type, name);
    *type = static_cast<GL::Enum>(tmp_type);
}

void gl::GetAttachedShaders(GL::UInt program, GL::Sizei maxcount, GL::Sizei* count, GL::UInt* shaders)
{
    g_OpenGLFunctions.glGetAttachedShaders(program, maxcount, count, shaders);
    Z_CHECK_GL_ERROR4(glGetAttachedShaders, program, maxcount, count, shaders);
}

int gl::GetAttribLocation(GL::UInt program, const GL::Char* name)
{
    int location = g_OpenGLFunctions.glGetAttribLocation(program, name);
    Z_CHECK_GL_ERROR2(glGetAttribLocation, program, name);
    Z_CHECK_GL_LOCATION(glGetAttribLocation, name, location)
    return location;
}

void gl::GetBooleanv(GL::Enum pname, GL::Boolean* params)
{
    glGetBooleanv(pname, params);
    Z_CHECK_GL_ERROR2(glGetBooleanv, pname, params);
}

void gl::GetBufferParameteriv(GL::Enum target, GL::Enum pname, GL::Int* params)
{
    g_OpenGLFunctions.glGetBufferParameteriv(target, pname, params);
    Z_CHECK_GL_ERROR3(glGetBufferParameteriv, target, pname, params);
}

GL::Enum gl::GetError()
{
    return static_cast<GL::Enum>(glGetError());
}

void gl::GetFloatv(GL::Enum pname, GL::Float* params)
{
    glGetFloatv(pname, params);
    Z_CHECK_GL_ERROR2(glGetFloatv, pname, params);
}

void gl::GetFramebufferAttachmentParameteriv(GL::Enum target, GL::Enum attachment, GL::Enum pname,
    GL::Int* params)
{
    g_OpenGLFunctions.glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
    Z_CHECK_GL_ERROR4(glGetFramebufferAttachmentParameteriv, target, attachment, pname, params);
}

void gl::GetIntegerv(GL::Enum pname, GL::Int* params)
{
    glGetIntegerv(pname, params);
    Z_CHECK_GL_ERROR2(glGetIntegerv, pname, params);
}

void gl::GetProgramiv(GL::UInt program, GL::Enum pname, GL::Int* params)
{
    g_OpenGLFunctions.glGetProgramiv(program, pname, params);
    Z_CHECK_GL_ERROR3(glGetProgramiv, program, pname, params);
}

void gl::GetProgramInfoLog(GL::UInt program, GL::Sizei bufsize, GL::Sizei* length, GL::Char* infolog)
{
    g_OpenGLFunctions.glGetProgramInfoLog(program, bufsize, length, infolog);
    Z_CHECK_GL_ERROR4(glGetProgramInfoLog, program, bufsize, length, infolog);
}

void gl::GetRenderbufferParameteriv(GL::Enum target, GL::Enum pname, GL::Int* params)
{
    g_OpenGLFunctions.glGetRenderbufferParameteriv(target, pname, params);
    Z_CHECK_GL_ERROR3(glGetRenderbufferParameteriv, target, pname, params);
}

void gl::GetShaderiv(GL::UInt shader, GL::Enum pname, GL::Int* params)
{
    g_OpenGLFunctions.glGetShaderiv(shader, pname, params);
    Z_CHECK_GL_ERROR3(glGetShaderiv, shader, pname, params);
}

void gl::GetShaderInfoLog(GL::UInt shader, GL::Sizei bufsize, GL::Sizei* length, GL::Char* infolog)
{
    g_OpenGLFunctions.glGetShaderInfoLog(shader, bufsize, length, infolog);
    Z_CHECK_GL_ERROR4(glGetShaderInfoLog, shader, bufsize, length, infolog);
}

void gl::GetShaderPrecisionFormat(GL::Enum shadertype, GL::Enum precisiontype, GL::Int* range, GL::Int* precision)
{
    g_OpenGLFunctions.glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
    Z_CHECK_GL_ERROR4(glGetShaderPrecisionFormat, shadertype, precisiontype, range, precision);
}

void gl::GetShaderSource(GL::UInt shader, GL::Sizei bufsize, GL::Sizei* length, GL::Char* source)
{
    g_OpenGLFunctions.glGetShaderSource(shader, bufsize, length, source);
    Z_CHECK_GL_ERROR4(glGetShaderSource, shader, bufsize, length, source);
}

const GL::Char* gl::GetString(GL::Enum name)
{
    const GL::Char* str = reinterpret_cast<const GL::Char*>(glGetString(name));
    Z_CHECK_GL_ERROR1(glGetString, name);
    return str;
}

void gl::GetTexParameterfv(GL::Enum target, GL::Enum pname, GL::Float* params)
{
    glGetTexParameterfv(target, pname, params);
    Z_CHECK_GL_ERROR3(glGetTexParameterfv, target, pname, params);
}

void gl::GetTexParameteriv(GL::Enum target, GL::Enum pname, GL::Int* params)
{
    glGetTexParameteriv(target, pname, params);
    Z_CHECK_GL_ERROR3(glGetTexParameteriv, target, pname, params);
}

void gl::GetUniformfv(GL::UInt program, GL::Int location, GL::Float* params)
{
    g_OpenGLFunctions.glGetUniformfv(program, location, params);
    Z_CHECK_GL_ERROR3(glGetUniformfv, program, location, params);
}

void gl::GetUniformiv(GL::UInt program, GL::Int location, GL::Int* params)
{
    g_OpenGLFunctions.glGetUniformiv(program, location, params);
    Z_CHECK_GL_ERROR3(glGetUniformiv, program, location, params);
}

int gl::GetUniformLocation(GL::UInt program, const GL::Char* name)
{
    int location = g_OpenGLFunctions.glGetUniformLocation(program, name);
    Z_CHECK_GL_ERROR2(glGetUniformLocation, program, name);
    Z_CHECK_GL_LOCATION(glGetUniformLocation, name, location)
    return location;
}

void gl::GetVertexAttribfv(GL::UInt index, GL::Enum pname, GL::Float* params)
{
    g_OpenGLFunctions.glGetVertexAttribfv(index, pname, params);
    Z_CHECK_GL_ERROR3(glGetVertexAttribfv, index, pname, params);
}

void gl::GetVertexAttribiv(GL::UInt index, GL::Enum pname, GL::Int* params)
{
    g_OpenGLFunctions.glGetVertexAttribiv(index, pname, params);
    Z_CHECK_GL_ERROR3(glGetVertexAttribiv, index, pname, params);
}

void gl::GetVertexAttribPointerv(GL::UInt index, GL::Enum pname, void** pointer)
{
    g_OpenGLFunctions.glGetVertexAttribPointerv(index, pname, pointer);
    Z_CHECK_GL_ERROR3(glGetVertexAttribPointerv, index, pname, pointer);
}

void gl::Hint(GL::Enum target, GL::Enum mode)
{
    glHint(target, mode);
    Z_CHECK_GL_ERROR2(glHint, target, mode);
}

GL::Boolean gl::IsBuffer(GL::UInt buffer)
{
    GL::Boolean flag = g_OpenGLFunctions.glIsBuffer(buffer);
    Z_CHECK_GL_ERROR1(glIsBuffer, buffer);
    return flag;
}

GL::Boolean gl::IsEnabled(GL::Enum cap)
{
    GL::Boolean flag = glIsEnabled(cap);
    Z_CHECK_GL_ERROR1(glIsEnabled, cap);
    return flag;
}

GL::Boolean gl::IsFramebuffer(GL::UInt framebuffer)
{
    GL::Boolean flag = g_OpenGLFunctions.glIsFramebuffer(framebuffer);
    Z_CHECK_GL_ERROR1(glIsFramebuffer, framebuffer);
    return flag;
}

GL::Boolean gl::IsProgram(GL::UInt program)
{
    GL::Boolean flag = g_OpenGLFunctions.glIsProgram(program);
    Z_CHECK_GL_ERROR1(glIsProgram, program);
    return flag;
}

GL::Boolean gl::IsRenderbuffer(GL::UInt renderbuffer)
{
    GL::Boolean flag = g_OpenGLFunctions.glIsRenderbuffer(renderbuffer);
    Z_CHECK_GL_ERROR1(glIsRenderbuffer, renderbuffer);
    return flag;
}

GL::Boolean gl::IsShader(GL::UInt shader)
{
    GL::Boolean flag = g_OpenGLFunctions.glIsShader(shader);
    Z_CHECK_GL_ERROR1(glIsShader, shader);
    return flag;
}

GL::Boolean gl::IsTexture(GL::UInt texture)
{
    GL::Boolean flag = glIsTexture(texture);
    Z_CHECK_GL_ERROR1(glIsTexture, texture);
    return flag;
}

void gl::LineWidth(GL::Float width)
{
    glLineWidth(width);
    Z_CHECK_GL_ERROR1(glLineWidth, width);
}

void gl::LinkProgram(GL::UInt program)
{
    g_OpenGLFunctions.glLinkProgram(program);
    Z_CHECK_GL_ERROR1(glLinkProgram, program);
}

void gl::PixelStorei(GL::Enum pname, GL::Int param)
{
    glPixelStorei(pname, param);
    Z_CHECK_GL_ERROR2(glPixelStorei, pname, param);
}

void gl::PolygonOffset(GL::Float factor, GL::Float units)
{
    glPolygonOffset(factor, units);
    Z_CHECK_GL_ERROR2(glPolygonOffset, factor, units);
}

void gl::ReadPixels(GL::Int x, GL::Int y, GL::Sizei width, GL::Sizei height, GL::Enum format, GL::Enum type,
    void* pixels)
{
    glReadPixels(x, y, width, height, format, type, pixels);
    Z_CHECK_GL_ERROR7(glReadPixels, x, y, width, height, format, type, pixels);
}

void gl::ReleaseShaderCompiler()
{
    g_OpenGLFunctions.glReleaseShaderCompiler();
    Z_CHECK_GL_ERROR0(glReleaseShaderCompiler);
}

void gl::RenderbufferStorage(GL::Enum target, GL::Enum internalformat, GL::Sizei width, GL::Sizei height)
{
    g_OpenGLFunctions.glRenderbufferStorage(target, internalformat, width, height);
    Z_CHECK_GL_ERROR4(glRenderbufferStorage, target, internalformat, width, height);
}

void gl::SampleCoverage(GL::Clampf value, GL::Boolean invert)
{
    g_OpenGLFunctions.glSampleCoverage(value, invert);
    Z_CHECK_GL_ERROR2(glSampleCoverage, value, invert);
}

void gl::Scissor(GL::Int x, GL::Int y, GL::Sizei width, GL::Sizei height)
{
    glScissor(x, y, width, height);
    Z_CHECK_GL_ERROR4(glScissor, x, y, width, height);
}

void gl::ShaderBinary(GL::Sizei n, const GL::UInt* shaders, GL::Enum binaryformat, const void* binary,
    GL::Sizei length)
{
    g_OpenGLFunctions.glShaderBinary(n, shaders, binaryformat, binary, length);
    Z_CHECK_GL_ERROR5(glShaderBinary, n, shaders, binaryformat, binary, length);
}

void gl::ShaderSource(GL::UInt shader, GL::Sizei count, const GL::Char** string, const GL::Int* length)
{
    g_OpenGLFunctions.glShaderSource(shader, count, string, length);
    Z_CHECK_GL_ERROR4(glShaderSource, shader, count, string, length);
}

void gl::StencilFunc(GL::Enum func, GL::Int ref, GL::UInt mask)
{
    glStencilFunc(func, ref, mask);
    Z_CHECK_GL_ERROR3(glStencilFunc, func, ref, mask);
}

void gl::StencilFuncSeparate(GL::Enum face, GL::Enum func, GL::Int ref, GL::UInt mask)
{
    g_OpenGLFunctions.glStencilFuncSeparate(face, func, ref, mask);
    Z_CHECK_GL_ERROR4(glStencilFuncSeparate, face, func, ref, mask);
}

void gl::StencilMask(GL::UInt mask)
{
    glStencilMask(mask);
    Z_CHECK_GL_ERROR1(glStencilMask, mask);
}

void gl::StencilMaskSeparate(GL::Enum face, GL::UInt mask)
{
    g_OpenGLFunctions.glStencilMaskSeparate(face, mask);
    Z_CHECK_GL_ERROR2(glStencilMaskSeparate, face, mask);
}

void gl::StencilOp(GL::Enum fail, GL::Enum zfail, GL::Enum zpass)
{
    glStencilOp(fail, zfail, zpass);
    Z_CHECK_GL_ERROR3(glStencilOp, fail, zfail, zpass);
}

void gl::StencilOpSeparate(GL::Enum face, GL::Enum fail, GL::Enum zfail, GL::Enum zpass)
{
    g_OpenGLFunctions.glStencilOpSeparate(face, fail, zfail, zpass);
    Z_CHECK_GL_ERROR4(glStencilOpSeparate, face, fail, zfail, zpass);
}

void gl::TexImage2D(GL::Enum target, GL::Int level, GL::Int internalformat, GL::Sizei width, GL::Sizei height,
    GL::Int border, GL::Enum format, GL::Enum type, const void* pixels)
{
    glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    Z_CHECK_GL_ERROR9(glTexImage2D, target, level, internalformat, width, height, border, format, type, pixels);
}

void gl::TexParameterf(GL::Enum target, GL::Enum pname, GL::Float param)
{
    glTexParameterf(target, pname, param);
    Z_CHECK_GL_ERROR3(glTexParameterf, target, pname, param);
}

void gl::TexParameterfv(GL::Enum target, GL::Enum pname, const GL::Float* params)
{
    glTexParameterfv(target, pname, params);
    Z_CHECK_GL_ERROR3(glTexParameterfv, target, pname, params);
}

void gl::TexParameteri(GL::Enum target, GL::Enum pname, GL::Int param)
{
    glTexParameteri(target, pname, param);
    Z_CHECK_GL_ERROR3(glTexParameteri, target, pname, param);
}

void gl::TexParameteriv(GL::Enum target, GL::Enum pname, const GL::Int* params)
{
    glTexParameteriv(target, pname, params);
    Z_CHECK_GL_ERROR3(glTexParameteriv, target, pname, params);
}

void gl::TexSubImage2D(GL::Enum target, GL::Int level, GL::Int xoffset, GL::Int yoffset, GL::Sizei width,
    GL::Sizei height, GL::Enum format, GL::Enum type, const void* pixels)
{
    glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
    Z_CHECK_GL_ERROR9(glTexSubImage2D, target, level, xoffset, yoffset, width, height, format, type, pixels);
}

void gl::Uniform1f(GL::Int location, GL::Float x)
{
    g_OpenGLFunctions.glUniform1f(location, x);
    Z_CHECK_GL_ERROR2(glUniform1f, location, x);
}

void gl::Uniform1fv(GL::Int location, GL::Sizei count, const GL::Float* v)
{
    g_OpenGLFunctions.glUniform1fv(location, count, v);
    Z_CHECK_GL_ERROR3(glUniform1fv, location, count, v);
}

void gl::Uniform1i(GL::Int location, GL::Int x)
{
    g_OpenGLFunctions.glUniform1i(location, x);
    Z_CHECK_GL_ERROR2(glUniform1i, location, x);
}

void gl::Uniform1iv(GL::Int location, GL::Sizei count, const GL::Int* v)
{
    g_OpenGLFunctions.glUniform1iv(location, count, v);
    Z_CHECK_GL_ERROR3(glUniform1iv, location, count, v);
}

void gl::Uniform2f(GL::Int location, GL::Float x, GL::Float y)
{
    g_OpenGLFunctions.glUniform2f(location, x, y);
    Z_CHECK_GL_ERROR3(glUniform2f, location, x, y);
}

void gl::Uniform2fv(GL::Int location, GL::Sizei count, const GL::Float* v)
{
    g_OpenGLFunctions.glUniform2fv(location, count, v);
    Z_CHECK_GL_ERROR3(glUniform2fv, location, count, v);
}

void gl::Uniform2i(GL::Int location, GL::Int x, GL::Int y)
{
    g_OpenGLFunctions.glUniform2i(location, x, y);
    Z_CHECK_GL_ERROR3(glUniform2i, location, x, y);
}

void gl::Uniform2iv(GL::Int location, GL::Sizei count, const GL::Int* v)
{
    g_OpenGLFunctions.glUniform2iv(location, count, v);
    Z_CHECK_GL_ERROR3(glUniform2iv, location, count, v);
}

void gl::Uniform3f(GL::Int location, GL::Float x, GL::Float y, GL::Float z)
{
    g_OpenGLFunctions.glUniform3f(location, x, y, z);
    Z_CHECK_GL_ERROR4(glUniform3f, location, x, y, z);
}

void gl::Uniform3fv(GL::Int location, GL::Sizei count, const GL::Float* v)
{
    g_OpenGLFunctions.glUniform3fv(location, count, v);
    Z_CHECK_GL_ERROR3(glUniform3fv, location, count, v);
}

void gl::Uniform3i(GL::Int location, GL::Int x, GL::Int y, GL::Int z)
{
    g_OpenGLFunctions.glUniform3i(location, x, y, z);
    Z_CHECK_GL_ERROR4(glUniform3i, location, x, y, z);
}

void gl::Uniform3iv(GL::Int location, GL::Sizei count, const GL::Int* v)
{
    g_OpenGLFunctions.glUniform3iv(location, count, v);
    Z_CHECK_GL_ERROR3(glUniform3iv, location, count, v);
}

void gl::Uniform4f(GL::Int location, GL::Float x, GL::Float y, GL::Float z, GL::Float w)
{
    g_OpenGLFunctions.glUniform4f(location, x, y, z, w);
    Z_CHECK_GL_ERROR5(glUniform4f, location, x, y, z, w);
}

void gl::Uniform4fv(GL::Int location, GL::Sizei count, const GL::Float* v)
{
    g_OpenGLFunctions.glUniform4fv(location, count, v);
    Z_CHECK_GL_ERROR3(glUniform4fv, location, count, v);
}

void gl::Uniform4i(GL::Int location, GL::Int x, GL::Int y, GL::Int z, GL::Int w)
{
    g_OpenGLFunctions.glUniform4i(location, x, y, z, w);
    Z_CHECK_GL_ERROR5(glUniform4i, location, x, y, z, w);
}

void gl::Uniform4iv(GL::Int location, GL::Sizei count, const GL::Int* v)
{
    g_OpenGLFunctions.glUniform4iv(location, count, v);
    Z_CHECK_GL_ERROR3(glUniform4iv, location, count, v);
}

void gl::UniformMatrix2fv(GL::Int location, GL::Sizei count, GL::Boolean transpose, const GL::Float* value)
{
    g_OpenGLFunctions.glUniformMatrix2fv(location, count, transpose, value);
    Z_CHECK_GL_ERROR4(glUniformMatrix2fv, location, count, transpose, value);
}

void gl::UniformMatrix3fv(GL::Int location, GL::Sizei count, GL::Boolean transpose, const GL::Float* value)
{
    g_OpenGLFunctions.glUniformMatrix3fv(location, count, transpose, value);
    Z_CHECK_GL_ERROR4(glUniformMatrix3fv, location, count, transpose, value);
}

void gl::UniformMatrix4fv(GL::Int location, GL::Sizei count, GL::Boolean transpose, const GL::Float* value)
{
    g_OpenGLFunctions.glUniformMatrix4fv(location, count, transpose, value);
    Z_CHECK_GL_ERROR4(glUniformMatrix4fv, location, count, transpose, value);
}

void gl::UseProgram(GL::UInt program)
{
    g_OpenGLFunctions.glUseProgram(program);
    Z_CHECK_GL_ERROR1(glUseProgram, program);
}

void gl::ValidateProgram(GL::UInt program)
{
    g_OpenGLFunctions.glValidateProgram(program);
    Z_CHECK_GL_ERROR1(glValidateProgram, program);
}

void gl::VertexAttrib1f(GL::UInt indx, GL::Float x)
{
    g_OpenGLFunctions.glVertexAttrib1f(indx, x);
    Z_CHECK_GL_ERROR2(glVertexAttrib1f, indx, x);
}

void gl::VertexAttrib1fv(GL::UInt indx, const GL::Float* values)
{
    g_OpenGLFunctions.glVertexAttrib1fv(indx, values);
    Z_CHECK_GL_ERROR2(glVertexAttrib1fv, indx, values);
}

void gl::VertexAttrib2f(GL::UInt indx, GL::Float x, GL::Float y)
{
    g_OpenGLFunctions.glVertexAttrib2f(indx, x, y);
    Z_CHECK_GL_ERROR3(glVertexAttrib2f, indx, x, y);
}

void gl::VertexAttrib2fv(GL::UInt indx, const GL::Float* values)
{
    g_OpenGLFunctions.glVertexAttrib2fv(indx, values);
    Z_CHECK_GL_ERROR2(glVertexAttrib2fv, indx, values);
}

void gl::VertexAttrib3f(GL::UInt indx, GL::Float x, GL::Float y, GL::Float z)
{
    g_OpenGLFunctions.glVertexAttrib3f(indx, x, y, z);
    Z_CHECK_GL_ERROR4(glVertexAttrib3f, indx, x, y, z);
}

void gl::VertexAttrib3fv(GL::UInt indx, const GL::Float* values)
{
    g_OpenGLFunctions.glVertexAttrib3fv(indx, values);
    Z_CHECK_GL_ERROR2(glVertexAttrib3fv, indx, values);
}

void gl::VertexAttrib4f(GL::UInt indx, GL::Float x, GL::Float y, GL::Float z, GL::Float w)
{
    g_OpenGLFunctions.glVertexAttrib4f(indx, x, y, z, w);
    Z_CHECK_GL_ERROR5(glVertexAttrib4f, indx, x, y, z, w);
}

void gl::VertexAttrib4fv(GL::UInt indx, const GL::Float* values)
{
    g_OpenGLFunctions.glVertexAttrib4fv(indx, values);
    Z_CHECK_GL_ERROR2(glVertexAttrib4fv, indx, values);
}

void gl::VertexAttribPointer(GL::UInt indx, GL::Int size, GL::Enum type, GL::Boolean norm, GL::Sizei stride,
    const void* ptr)
{
    g_OpenGLFunctions.glVertexAttribPointer(indx, size, type, norm, stride, ptr);
    Z_CHECK_GL_ERROR6(glVertexAttribPointer, indx, size, type, norm, stride, ptr);
}

void gl::Viewport(GL::Int x, GL::Int y, GL::Sizei width, GL::Sizei height)
{
    glViewport(x, y, width, height);
    Z_CHECK_GL_ERROR4(glViewport, x, y, width, height);
}
