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

void gl::InitWrappers()
{
}

void gl::ActiveTexture(GL::Enum texture)
{
    (void)texture;
}

void gl::AttachShader(GL::UInt program, GL::UInt shader)
{
    (void)program;
    (void)shader;
}

void gl::BindAttribLocation(GL::UInt program, GL::UInt index, const GL::Char* name)
{
    (void)program;
    (void)index;
    (void)name;
}

void gl::BindBuffer(GL::Enum target, GL::UInt buffer)
{
    (void)target;
    (void)buffer;
}

void gl::BindFramebuffer(GL::Enum target, GL::UInt framebuffer)
{
    (void)target;
    (void)framebuffer;
}

void gl::BindRenderbuffer(GL::Enum target, GL::UInt renderbuffer)
{
    (void)target;
    (void)renderbuffer;
}

void gl::BindTexture(GL::Enum target, GL::UInt texture)
{
    (void)target;
    (void)texture;
}

void gl::BlendColor(GL::Clampf red, GL::Clampf green, GL::Clampf blue, GL::Clampf alpha)
{
    (void)red;
    (void)green;
    (void)blue;
    (void)alpha;
}

void gl::BlendEquation(GL::Enum mode)
{
    (void)mode;
}

void gl::BlendEquationSeparate(GL::Enum modeRGB, GL::Enum modeAlpha)
{
    (void)modeRGB;
    (void)modeAlpha;
}

void gl::BlendFunc(GL::Enum sfactor, GL::Enum dfactor)
{
    (void)sfactor;
    (void)dfactor;
}

void gl::BlendFuncSeparate(GL::Enum srcRGB, GL::Enum dstRGB, GL::Enum srcAlpha, GL::Enum dstAlpha)
{
    (void)srcRGB;
    (void)dstRGB;
    (void)srcAlpha;
    (void)dstAlpha;
}

void gl::BufferData(GL::Enum target, GL::Sizeiptr size, const void* data, GL::Enum usage)
{
    (void)target;
    (void)size;
    (void)data;
    (void)usage;
}

void gl::BufferSubData(GL::Enum target, GL::Intptr offset, GL::Sizeiptr size, const void* data)
{
    (void)target;
    (void)offset;
    (void)size;
    (void)data;
}

GL::Enum gl::CheckFramebufferStatus(GL::Enum target)
{
    (void)target;
    return GL::FRAMEBUFFER_UNSUPPORTED;
}

void gl::Clear(GL::Bitfield mask)
{
    (void)mask;
}

void gl::ClearColor(GL::Clampf red, GL::Clampf green, GL::Clampf blue, GL::Clampf alpha)
{
    (void)red;
    (void)green;
    (void)blue;
    (void)alpha;
}

void gl::ClearDepthf(GL::Clampf depth)
{
    (void)depth;
}

void gl::ClearStencil(GL::Int s)
{
    (void)s;
}

void gl::ColorMask(GL::Boolean red, GL::Boolean green, GL::Boolean blue, GL::Boolean alpha)
{
    (void)red;
    (void)green;
    (void)blue;
    (void)alpha;
}

void gl::CompileShader(GL::UInt shader)
{
    (void)shader;
}

void gl::CompressedTexImage2D(GL::Enum target, GL::Int level, GL::Enum internalformat, GL::Sizei width,
    GL::Sizei height, GL::Int border, GL::Sizei imageSize, const void* data)
{
    (void)target;
    (void)level;
    (void)internalformat;
    (void)width;
    (void)height;
    (void)border;
    (void)imageSize;
    (void)data;
}

void gl::CompressedTexSubImage2D(GL::Enum target, GL::Int level, GL::Int xoffset, GL::Int yoffset,
    GL::Sizei width, GL::Sizei height, GL::Enum format, GL::Sizei imageSize, const void* data)
{
    (void)target;
    (void)level;
    (void)xoffset;
    (void)yoffset;
    (void)width;
    (void)height;
    (void)format;
    (void)imageSize;
    (void)data;
}

void gl::CopyTexImage2D(GL::Enum target, GL::Int level, GL::Enum intfmt, GL::Int x, GL::Int y, GL::Sizei w,
    GL::Sizei h, GL::Int border)
{
    (void)target;
    (void)level;
    (void)intfmt;
    (void)x;
    (void)y;
    (void)w;
    (void)h;
    (void)border;
}

void gl::CopyTexSubImage2D(GL::Enum target, GL::Int level, GL::Int xoff, GL::Int yoff, GL::Int x, GL::Int y,
    GL::Sizei w, GL::Sizei h)
{
    (void)target;
    (void)level;
    (void)xoff;
    (void)yoff;
    (void)x;
    (void)y;
    (void)w;
    (void)h;
}

GL::UInt gl::CreateProgram()
{
    return 0;
}

GL::UInt gl::CreateShader(GL::Enum type)
{
    (void)type;
    return 0;
}

void gl::CullFace(GL::Enum mode)
{
    (void)mode;
}

void gl::DeleteBuffers(GL::Sizei n, const GL::UInt* buffers)
{
    (void)n;
    (void)buffers;
}

void gl::DeleteFramebuffers(GL::Sizei n, const GL::UInt* framebuffers)
{
    (void)n;
    (void)framebuffers;
}

void gl::DeleteProgram(GL::UInt program)
{
    (void)program;
}

void gl::DeleteRenderbuffers(GL::Sizei n, const GL::UInt* renderbuffers)
{
    (void)n;
    (void)renderbuffers;
}

void gl::DeleteShader(GL::UInt shader)
{
    (void)shader;
}

void gl::DeleteTextures(GL::Sizei n, const GL::UInt* textures)
{
    (void)n;
    (void)textures;
}

void gl::DepthFunc(GL::Enum func)
{
    (void)func;
}

void gl::DepthMask(GL::Boolean flag)
{
    (void)flag;
}

void gl::DepthRangef(GL::Clampf zNear, GL::Clampf zFar)
{
    (void)zNear;
    (void)zFar;
}

void gl::DetachShader(GL::UInt program, GL::UInt shader)
{
    (void)program;
    (void)shader;
}

void gl::Disable(GL::Enum cap)
{
    (void)cap;
}

void gl::DisableVertexAttribArray(GL::UInt index)
{
    (void)index;
}

void gl::DrawArrays(GL::Enum mode, GL::Int first, GL::Sizei count)
{
    (void)mode;
    (void)first;
    (void)count;
}

void gl::DrawElements(GL::Enum mode, GL::Sizei count, GL::Enum type, const void* indices)
{
    (void)mode;
    (void)count;
    (void)type;
    (void)indices;
}

void gl::Enable(GL::Enum cap)
{
    (void)cap;
}

void gl::EnableVertexAttribArray(GL::UInt index)
{
    (void)index;
}

void gl::Finish()
{
}

void gl::Flush()
{
}

void gl::FramebufferRenderbuffer(GL::Enum target, GL::Enum attachment, GL::Enum renderbuffertarget,
    GL::UInt renderbuffer)
{
    (void)target;
    (void)attachment;
    (void)renderbuffertarget;
    (void)renderbuffer;
}

void gl::FramebufferTexture2D(GL::Enum target, GL::Enum attachment, GL::Enum textarget, GL::UInt texture,
    GL::Int level)
{
    (void)target;
    (void)attachment;
    (void)textarget;
    (void)texture;
    (void)level;
}

void gl::FrontFace(GL::Enum mode)
{
    (void)mode;
}

void gl::GenBuffers(GL::Sizei n, GL::UInt* buffers)
{
    (void)n;
    (void)buffers;
}

void gl::GenerateMipmap(GL::Enum target)
{
    (void)target;
}

void gl::GenFramebuffers(GL::Sizei n, GL::UInt* framebuffers)
{
    (void)n;
    (void)framebuffers;
}

void gl::GenRenderbuffers(GL::Sizei n, GL::UInt* renderbuffers)
{
    (void)n;
    (void)renderbuffers;
}

void gl::GenTextures(GL::Sizei n, GL::UInt* textures)
{
    (void)n;
    (void)textures;
}

void gl::GetActiveAttrib(GL::UInt prog, GL::UInt idx, GL::Sizei bufsz, GL::Sizei* len, GL::Int* sz,
    GL::Enum* type, GL::Char* name)
{
    (void)prog;
    (void)idx;
    (void)bufsz;
    (void)len;
    (void)sz;
    (void)type;
    (void)name;
}

void gl::GetActiveUniform(GL::UInt prog, GL::UInt idx, GL::Sizei bufsz, GL::Sizei* len, GL::Int* sz,
    GL::Enum* type, GL::Char* name)
{
    (void)prog;
    (void)idx;
    (void)bufsz;
    (void)len;
    (void)sz;
    (void)type;
    (void)name;
}

void gl::GetAttachedShaders(GL::UInt program, GL::Sizei maxcount, GL::Sizei* count, GL::UInt* shaders)
{
    (void)program;
    (void)maxcount;
    (void)count;
    (void)shaders;
}

int gl::GetAttribLocation(GL::UInt program, const GL::Char* name)
{
    (void)program;
    (void)name;
    return -1;
}

void gl::GetBooleanv(GL::Enum pname, GL::Boolean* params)
{
    (void)pname;
    (void)params;
}

void gl::GetBufferParameteriv(GL::Enum target, GL::Enum pname, GL::Int* params)
{
    (void)target;
    (void)pname;
    (void)params;
}

GL::Enum gl::GetError()
{
    return GL::INVALID_OPERATION;
}

void gl::GetFloatv(GL::Enum pname, GL::Float* params)
{
    (void)pname;
    (void)params;
}

void gl::GetFramebufferAttachmentParameteriv(GL::Enum target, GL::Enum attachment, GL::Enum pname,
    GL::Int* params)
{
    (void)target;
    (void)attachment;
    (void)pname;
    (void)params;
}

void gl::GetIntegerv(GL::Enum pname, GL::Int* params)
{
    (void)pname;
    (void)params;
}

void gl::GetProgramiv(GL::UInt program, GL::Enum pname, GL::Int* params)
{
    (void)program;
    (void)pname;
    (void)params;
}

void gl::GetProgramInfoLog(GL::UInt program, GL::Sizei bufsize, GL::Sizei* length, GL::Char* infolog)
{
    (void)program;
    (void)bufsize;
    (void)length;
    (void)infolog;
}

void gl::GetRenderbufferParameteriv(GL::Enum target, GL::Enum pname, GL::Int* params)
{
    (void)target;
    (void)pname;
    (void)params;
}

void gl::GetShaderiv(GL::UInt shader, GL::Enum pname, GL::Int* params)
{
    (void)shader;
    (void)pname;
    (void)params;
}

void gl::GetShaderInfoLog(GL::UInt shader, GL::Sizei bufsize, GL::Sizei* length, GL::Char* infolog)
{
    (void)shader;
    (void)bufsize;
    (void)length;
    (void)infolog;
}

void gl::GetShaderPrecisionFormat(GL::Enum shadertype, GL::Enum precisiontype, GL::Int* range, GL::Int* precision)
{
    (void)shadertype;
    (void)precisiontype;
    (void)range;
    (void)precision;
}

void gl::GetShaderSource(GL::UInt shader, GL::Sizei bufsize, GL::Sizei* length, GL::Char* source)
{
    (void)shader;
    (void)bufsize;
    (void)length;
    (void)source;
}

const GL::Char* gl::GetString(GL::Enum name)
{
    (void)name;
    return "";
}

void gl::GetTexParameterfv(GL::Enum target, GL::Enum pname, GL::Float* params)
{
    (void)target;
    (void)pname;
    (void)params;
}

void gl::GetTexParameteriv(GL::Enum target, GL::Enum pname, GL::Int* params)
{
    (void)target;
    (void)pname;
    (void)params;
}

void gl::GetUniformfv(GL::UInt program, GL::Int location, GL::Float* params)
{
    (void)program;
    (void)location;
    (void)params;
}

void gl::GetUniformiv(GL::UInt program, GL::Int location, GL::Int* params)
{
    (void)program;
    (void)location;
    (void)params;
}

int gl::GetUniformLocation(GL::UInt program, const GL::Char* name)
{
    (void)program;
    (void)name;
    return -1;
}

void gl::GetVertexAttribfv(GL::UInt index, GL::Enum pname, GL::Float* params)
{
    (void)index;
    (void)pname;
    (void)params;
}

void gl::GetVertexAttribiv(GL::UInt index, GL::Enum pname, GL::Int* params)
{
    (void)index;
    (void)pname;
    (void)params;
}

void gl::GetVertexAttribPointerv(GL::UInt index, GL::Enum pname, void** pointer)
{
    (void)index;
    (void)pname;
    (void)pointer;
}

void gl::Hint(GL::Enum target, GL::Enum mode)
{
    (void)target;
    (void)mode;
}

GL::Boolean gl::IsBuffer(GL::UInt buffer)
{
    (void)buffer;
    return GL::FALSE;
}

GL::Boolean gl::IsEnabled(GL::Enum cap)
{
    (void)cap;
    return GL::FALSE;
}

GL::Boolean gl::IsFramebuffer(GL::UInt framebuffer)
{
    (void)framebuffer;
    return GL::FALSE;
}

GL::Boolean gl::IsProgram(GL::UInt program)
{
    (void)program;
    return GL::FALSE;
}

GL::Boolean gl::IsRenderbuffer(GL::UInt renderbuffer)
{
    (void)renderbuffer;
    return GL::FALSE;
}

GL::Boolean gl::IsShader(GL::UInt shader)
{
    (void)shader;
    return GL::FALSE;
}

GL::Boolean gl::IsTexture(GL::UInt texture)
{
    (void)texture;
    return GL::FALSE;
}

void gl::LineWidth(GL::Float width)
{
    (void)width;
}

void gl::LinkProgram(GL::UInt program)
{
    (void)program;
}

void gl::PixelStorei(GL::Enum pname, GL::Int param)
{
    (void)pname;
    (void)param;
}

void gl::PolygonOffset(GL::Float factor, GL::Float units)
{
    (void)factor;
    (void)units;
}

void gl::ReadPixels(GL::Int x, GL::Int y, GL::Sizei width, GL::Sizei height, GL::Enum format, GL::Enum type,
    void* pixels)
{
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)format;
    (void)type;
    (void)pixels;
}

void gl::ReleaseShaderCompiler()
{
}

void gl::RenderbufferStorage(GL::Enum target, GL::Enum internalformat, GL::Sizei width, GL::Sizei height)
{
    (void)target;
    (void)internalformat;
    (void)width;
    (void)height;
}

void gl::SampleCoverage(GL::Clampf value, GL::Boolean invert)
{
    (void)value;
    (void)invert;
}

void gl::Scissor(GL::Int x, GL::Int y, GL::Sizei width, GL::Sizei height)
{
    (void)x;
    (void)y;
    (void)width;
    (void)height;
}

void gl::ShaderBinary(GL::Sizei n, const GL::UInt* shaders, GL::Enum binaryformat, const void* binary,
    GL::Sizei length)
{
    (void)n;
    (void)shaders;
    (void)binaryformat;
    (void)binary;
    (void)length;
}

void gl::ShaderSource(GL::UInt shader, GL::Sizei count, const GL::Char** string, const GL::Int* length)
{
    (void)shader;
    (void)count;
    (void)string;
    (void)length;
}

void gl::StencilFunc(GL::Enum func, GL::Int ref, GL::UInt mask)
{
    (void)func;
    (void)ref;
    (void)mask;
}

void gl::StencilFuncSeparate(GL::Enum face, GL::Enum func, GL::Int ref, GL::UInt mask)
{
    (void)face;
    (void)func;
    (void)ref;
    (void)mask;
}

void gl::StencilMask(GL::UInt mask)
{
    (void)mask;
}

void gl::StencilMaskSeparate(GL::Enum face, GL::UInt mask)
{
    (void)face;
    (void)mask;
}

void gl::StencilOp(GL::Enum fail, GL::Enum zfail, GL::Enum zpass)
{
    (void)fail;
    (void)zfail;
    (void)zpass;
}

void gl::StencilOpSeparate(GL::Enum face, GL::Enum fail, GL::Enum zfail, GL::Enum zpass)
{
    (void)face;
    (void)fail;
    (void)zfail;
    (void)zpass;
}

void gl::TexImage2D(GL::Enum target, GL::Int level, GL::Int internalformat, GL::Sizei width, GL::Sizei height,
    GL::Int border, GL::Enum format, GL::Enum type, const void* pixels)
{
    (void)target;
    (void)level;
    (void)internalformat;
    (void)width;
    (void)height;
    (void)border;
    (void)format;
    (void)type;
    (void)pixels;
}

void gl::TexParameterf(GL::Enum target, GL::Enum pname, GL::Float param)
{
    (void)target;
    (void)pname;
    (void)param;
}

void gl::TexParameterfv(GL::Enum target, GL::Enum pname, const GL::Float* params)
{
    (void)target;
    (void)pname;
    (void)params;
}

void gl::TexParameteri(GL::Enum target, GL::Enum pname, GL::Int param)
{
    (void)target;
    (void)pname;
    (void)param;
}

void gl::TexParameteriv(GL::Enum target, GL::Enum pname, const GL::Int* params)
{
    (void)target;
    (void)pname;
    (void)params;
}

void gl::TexSubImage2D(GL::Enum target, GL::Int level, GL::Int xoffset, GL::Int yoffset, GL::Sizei width,
    GL::Sizei height, GL::Enum format, GL::Enum type, const void* pixels)
{
    (void)target;
    (void)level;
    (void)xoffset;
    (void)yoffset;
    (void)width;
    (void)height;
    (void)format;
    (void)type;
    (void)pixels;
}

void gl::Uniform1f(GL::Int location, GL::Float x)
{
    (void)location;
    (void)x;
}

void gl::Uniform1fv(GL::Int location, GL::Sizei count, const GL::Float* v)
{
    (void)location;
    (void)count;
    (void)v;
}

void gl::Uniform1i(GL::Int location, GL::Int x)
{
    (void)location;
    (void)x;
}

void gl::Uniform1iv(GL::Int location, GL::Sizei count, const GL::Int* v)
{
    (void)location;
    (void)count;
    (void)v;
}

void gl::Uniform2f(GL::Int location, GL::Float x, GL::Float y)
{
    (void)location;
    (void)x;
    (void)y;
}

void gl::Uniform2fv(GL::Int location, GL::Sizei count, const GL::Float* v)
{
    (void)location;
    (void)count;
    (void)v;
}

void gl::Uniform2i(GL::Int location, GL::Int x, GL::Int y)
{
    (void)location;
    (void)x;
    (void)y;
}

void gl::Uniform2iv(GL::Int location, GL::Sizei count, const GL::Int* v)
{
    (void)location;
    (void)count;
    (void)v;
}

void gl::Uniform3f(GL::Int location, GL::Float x, GL::Float y, GL::Float z)
{
    (void)location;
    (void)x;
    (void)y;
    (void)z;
}

void gl::Uniform3fv(GL::Int location, GL::Sizei count, const GL::Float* v)
{
    (void)location;
    (void)count;
    (void)v;
}

void gl::Uniform3i(GL::Int location, GL::Int x, GL::Int y, GL::Int z)
{
    (void)location;
    (void)x;
    (void)y;
    (void)z;
}

void gl::Uniform3iv(GL::Int location, GL::Sizei count, const GL::Int* v)
{
    (void)location;
    (void)count;
    (void)v;
}

void gl::Uniform4f(GL::Int location, GL::Float x, GL::Float y, GL::Float z, GL::Float w)
{
    (void)location;
    (void)x;
    (void)y;
    (void)z;
    (void)w;
}

void gl::Uniform4fv(GL::Int location, GL::Sizei count, const GL::Float* v)
{
    (void)location;
    (void)count;
    (void)v;
}

void gl::Uniform4i(GL::Int location, GL::Int x, GL::Int y, GL::Int z, GL::Int w)
{
    (void)location;
    (void)x;
    (void)y;
    (void)z;
    (void)w;
}

void gl::Uniform4iv(GL::Int location, GL::Sizei count, const GL::Int* v)
{
    (void)location;
    (void)count;
    (void)v;
}

void gl::UniformMatrix2fv(GL::Int location, GL::Sizei count, GL::Boolean transpose, const GL::Float* value)
{
    (void)location;
    (void)count;
    (void)transpose;
    (void)value;
}

void gl::UniformMatrix3fv(GL::Int location, GL::Sizei count, GL::Boolean transpose, const GL::Float* value)
{
    (void)location;
    (void)count;
    (void)transpose;
    (void)value;
}

void gl::UniformMatrix4fv(GL::Int location, GL::Sizei count, GL::Boolean transpose, const GL::Float* value)
{
    (void)location;
    (void)count;
    (void)transpose;
    (void)value;
}

void gl::UseProgram(GL::UInt program)
{
    (void)program;
}

void gl::ValidateProgram(GL::UInt program)
{
    (void)program;
}

void gl::VertexAttrib1f(GL::UInt indx, GL::Float x)
{
    (void)indx;
    (void)x;
}

void gl::VertexAttrib1fv(GL::UInt indx, const GL::Float* values)
{
    (void)indx;
    (void)values;
}

void gl::VertexAttrib2f(GL::UInt indx, GL::Float x, GL::Float y)
{
    (void)indx;
    (void)x;
    (void)y;
}

void gl::VertexAttrib2fv(GL::UInt indx, const GL::Float* values)
{
    (void)indx;
    (void)values;
}

void gl::VertexAttrib3f(GL::UInt indx, GL::Float x, GL::Float y, GL::Float z)
{
    (void)indx;
    (void)x;
    (void)y;
    (void)z;
}

void gl::VertexAttrib3fv(GL::UInt indx, const GL::Float* values)
{
    (void)indx;
    (void)values;
}

void gl::VertexAttrib4f(GL::UInt indx, GL::Float x, GL::Float y, GL::Float z, GL::Float w)
{
    (void)indx;
    (void)x;
    (void)y;
    (void)z;
    (void)w;
}

void gl::VertexAttrib4fv(GL::UInt indx, const GL::Float* values)
{
    (void)indx;
    (void)values;
}

void gl::VertexAttribPointer(GL::UInt indx, GL::Int size, GL::Enum type, GL::Boolean norm, GL::Sizei stride,
    const void* ptr)
{
    (void)indx;
    (void)size;
    (void)type;
    (void)norm;
    (void)stride;
    (void)ptr;
}

void gl::Viewport(GL::Int x, GL::Int y, GL::Sizei width, GL::Sizei height)
{
    (void)x;
    (void)y;
    (void)width;
    (void)height;
}
