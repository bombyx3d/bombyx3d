//
// Copyright (c) 2013-2014 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// formatutils11.h: Queries for GL image formats and their translations to D3D11
// formats.

#ifndef LIBANGLE_RENDERER_D3D_D3D11_FORMATUTILS11_H_
#define LIBANGLE_RENDERER_D3D_D3D11_FORMATUTILS11_H_

#include <map>

#include "common/platform.h"
#include "libANGLE/angletypes.h"
#include "libANGLE/formatutils.h"
#include "libANGLE/renderer/d3d/formatutilsD3D.h"

namespace rx
{
struct Renderer11DeviceCaps;

namespace d3d11
{

typedef std::map<std::pair<GLenum, GLenum>, ColorCopyFunction> FastCopyFunctionMap;
typedef bool (*NativeMipmapGenerationSupportFunction)(D3D_FEATURE_LEVEL);

struct DXGIFormat
{
    DXGIFormat();

    GLuint pixelBytes;
    GLuint blockWidth;
    GLuint blockHeight;

    GLuint redBits;
    GLuint greenBits;
    GLuint blueBits;
    GLuint alphaBits;
    GLuint sharedBits;

    GLuint depthBits;
    GLuint depthOffset;
    GLuint stencilBits;
    GLuint stencilOffset;

    GLenum internalFormat;
    GLenum componentType;

    MipGenerationFunction mipGenerationFunction;
    ColorReadFunction colorReadFunction;

    FastCopyFunctionMap fastCopyFunctions;

    NativeMipmapGenerationSupportFunction nativeMipmapSupport;

    ColorCopyFunction getFastCopyFunction(GLenum format, GLenum type) const;
};
const DXGIFormat &GetDXGIFormatInfo(DXGI_FORMAT format);

struct TextureFormat
{
    TextureFormat();

    DXGI_FORMAT texFormat;
    DXGI_FORMAT srvFormat;
    DXGI_FORMAT rtvFormat;
    DXGI_FORMAT dsvFormat;
    DXGI_FORMAT renderFormat;

    DXGI_FORMAT swizzleTexFormat;
    DXGI_FORMAT swizzleSRVFormat;
    DXGI_FORMAT swizzleRTVFormat;

    InitializeTextureDataFunction dataInitializerFunction;

    typedef std::map<GLenum, LoadImageFunction> LoadFunctionMap;
    LoadFunctionMap loadFunctions;
};
const TextureFormat &GetTextureFormatInfo(GLenum internalFormat, const Renderer11DeviceCaps &renderer11DeviceCaps, bool renderable);

struct VertexFormat
{
    VertexFormat();
    VertexFormat(VertexConversionType conversionType,
                 DXGI_FORMAT nativeFormat,
                 VertexCopyFunction copyFunction);

    VertexConversionType conversionType;
    DXGI_FORMAT nativeFormat;
    VertexCopyFunction copyFunction;
};
const VertexFormat &GetVertexFormatInfo(gl::VertexFormatType vertexFormatType, D3D_FEATURE_LEVEL featureLevel);

}

}

#endif // LIBANGLE_RENDERER_D3D_D3D11_FORMATUTILS11_H_
