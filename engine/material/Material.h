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
#include "engine/core/macros.h"
#include "engine/interfaces/material/IMaterial.h"
#include "engine/interfaces/material/IMaterialLoader.h"
#include "engine/interfaces/io/IFile.h"
#include <string>
#include <memory>
#include <mutex>
#include <vector>

namespace B3D
{
    class Material : public IMaterial
    {
    public:
        static const std::string DEFAULT_FILE_EXTENSION;

        Material();
        ~Material();

        size_t numTechniques() const override;
        const MaterialTechniquePtr& technique(const std::string& name) const override;
        const MaterialTechniquePtr& technique(size_t index) const override;

        void addTechnique(const MaterialTechniquePtr& technique);
        void addTechnique(MaterialTechniquePtr&& technique);

        bool load(const std::string& fileName);
        bool load(const FilePtr& file);
        bool load(IFile* file);

        void loadPendingResources(bool async);

        static MaterialPtr fromFile(const std::string& fileName);
        static MaterialPtr fromFile(const FilePtr& file);
        static MaterialPtr fromFile(IFile* file);

        static void registerLoader(std::unique_ptr<IMaterialLoader>&& loader);
        template <typename TYPE, typename... ARGS> static void registerLoader(ARGS&&... args)
            { registerLoader(std::unique_ptr<TYPE>(new TYPE(std::forward<ARGS>(args)...))); }

    private:
        static std::vector<std::unique_ptr<IMaterialLoader>> mMaterialLoaders;
        static std::mutex mMaterialLoadersMutex;

        std::vector<MaterialTechniquePtr> mTechniques;

        B3D_DISABLE_COPY(Material);
    };
}
