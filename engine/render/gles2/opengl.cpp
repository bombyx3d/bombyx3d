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
#include "opengl.h"
#include <cassert>

namespace B3D
{
    GLenum primitiveTypeToGL(PrimitiveType primitiveType)
    {
        switch (primitiveType)
        {
        case PrimitiveType::Points: return GL_POINTS;
        case PrimitiveType::Lines: return GL_LINES;
        case PrimitiveType::LineStrip: return GL_LINE_STRIP;
        case PrimitiveType::Triangles: return GL_TRIANGLES;
        case PrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
        }

        assert(false);
        return GL_POINTS;
    }

    GLenum cullFaceToGL(CullFace face)
    {
        switch (face)
        {
        case CullFace::None: return GL_NONE;
        case CullFace::Front: return GL_FRONT;
        case CullFace::Back: return GL_BACK;
        }

        assert(false);
        return GL_NONE;
    }

    GLenum frontFaceToGL(FrontFace face)
    {
        switch (face)
        {
        case FrontFace::Clockwise: return GL_CW;
        case FrontFace::CounterClockwise: return GL_CCW;
        }

        assert(false);
        return GL_CW;
    }

    GLenum blendFuncToGL(BlendFunc func)
    {
        switch (func)
        {
        case BlendFunc::Zero: return GL_ZERO;
        case BlendFunc::One: return GL_ONE;
        case BlendFunc::SrcColor: return GL_SRC_COLOR;
        case BlendFunc::OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
        case BlendFunc::DstColor: return GL_DST_COLOR;
        case BlendFunc::OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
        case BlendFunc::SrcAlpha: return GL_SRC_ALPHA;
        case BlendFunc::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
        case BlendFunc::DstAlpha: return GL_DST_ALPHA;
        case BlendFunc::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
        case BlendFunc::SrcAlphaSaturate: return GL_SRC_ALPHA_SATURATE;
        }

        assert(false);
        return GL_SRC_ALPHA;
    }

    GLenum bufferUsageToGL(BufferUsage usage)
    {
        switch (usage)
        {
        case BufferUsage::Static: return GL_STATIC_DRAW;
        case BufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
        case BufferUsage::Stream: return GL_STREAM_DRAW;
        }

        assert(false);
        return GL_STATIC_DRAW;
    }
}
