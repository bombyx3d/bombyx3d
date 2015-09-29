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
#include "engine/interfaces/material/IMaterial.h"
#include "engine/interfaces/image/ISpriteSheet.h"
#include "engine/interfaces/mesh/IMesh.h"
#include "engine/interfaces/render/lowlevel/IShader.h"
#include "engine/interfaces/render/lowlevel/ITexture.h"
#include <memory>
#include <vector>
#include <string>

namespace B3D
{
    class IResourceManager
    {
    public:
        virtual ~IResourceManager() = default;

        virtual bool resourcesAreLoading() const = 0;
        virtual float resourceLoadProgress() const = 0;

        virtual ShaderPtr compileShader(const std::vector<std::string>* source,
            const std::string& fileName = "<builtin>") = 0;

        virtual MaterialPtr getMaterial(const std::string& fileName, bool async = true) = 0;
        virtual ShaderPtr getShader(const std::string& fileName, bool async = true) = 0;
        virtual TexturePtr getTexture(const std::string& fileName, bool async = true) = 0;
        virtual SpriteSheetPtr getSpriteSheet(const std::string& fileName, bool async = true) = 0;
        virtual MeshPtr getStaticMesh(const std::string& fileName, bool async = true) = 0;
    };

    using ResourceManagerPtr = std::shared_ptr<IResourceManager>;
}
