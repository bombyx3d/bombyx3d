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

#ifdef __APPLE__
 #include <OpenGL/gl.h>
#elif defined(B3D_TARGET_WINRT)
 #include <GLES2/gl2.h>
#elif defined(_WIN32)
 #include "engine/platform/win32/WinAPI.h"
 #ifdef B3D_USE_ANGLE
  #include <GLES2/gl2.h>
 #else
  #include <GL/glew.h>
  #include <GL/gl.h>
 #endif
#elif defined(__linux__)
 #include <GL/glew.h>
 #include <GL/gl.h>
#else
 #include <GL/gl.h>
#endif

#include "engine/interfaces/render/lowlevel/IRenderer.h"
#include "engine/interfaces/render/lowlevel/IBuffer.h"

namespace B3D
{
    GLenum primitiveTypeToGL(PrimitiveType primitiveType);
    GLenum cullFaceToGL(CullFace face);
    GLenum frontFaceToGL(FrontFace face);
    GLenum blendFuncToGL(BlendFunc func);
    GLenum bufferUsageToGL(BufferUsage usage);
}
