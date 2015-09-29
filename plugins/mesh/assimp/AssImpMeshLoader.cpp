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
#include "AssImpMeshLoader.h"
#include "AssImpLogStream.h"
#include "AssImpIOSystem.h"
#include "AssImpIOStream.h"
#include "engine/mesh/RawMeshData.h"
#include "engine/mesh/RawMeshElementData.h"
#include "engine/mesh/VertexFormat.h"
#include "engine/material/Material.h"
#include "engine/core/Log.h"
#include "engine/core/Services.h"
#include "engine/utility/StringUtils.h"
#include "engine/utility/FileUtils.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Engine
{
    namespace
    {
        static const int MAX_VERTICES_PER_ELEMENT = 65534;

        B3D_VERTEX_FORMAT(Vertex,
            (glm::vec3) position,
            (glm::vec3) normal,
            (glm::vec3) tangent,
            (glm::vec3) bitangent,
            (glm::vec2) texCoord
        )
    }

    AssImpMeshLoader::AssImpMeshLoader()
    {
        AssImpLogStream::init();
    }

    bool AssImpMeshLoader::canLoadMesh(IFile*)
    {
        // FIXME
        return true;
    }

    RawMeshDataPtr AssImpMeshLoader::loadMesh(IFile* file, bool loadSkeleton)
    {
        auto mesh = std::make_shared<RawMeshData>();

        if (!file)
            return mesh;

        const aiScene* scene = nullptr;
        const unsigned flags =
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_ImproveCacheLocality |
            aiProcess_RemoveRedundantMaterials |
            aiProcess_SortByPType |
            aiProcess_FindInvalidData |
            aiProcess_SplitLargeMeshes |
            aiProcess_OptimizeMeshes |
            (!loadSkeleton ? aiProcess_PreTransformVertices : 0) |
            (loadSkeleton ? aiProcess_LimitBoneWeights : 0) |
            aiProcess_GenSmoothNormals |
            aiProcess_CalcTangentSpace |
            aiProcess_GenUVCoords |
            aiProcess_TransformUVCoords |
            aiProcess_FlipUVs |
            0;

        Assimp::Importer importer;
        importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, MAX_VERTICES_PER_ELEMENT);
        importer.SetPropertyInteger(AI_CONFIG_PP_SLM_TRIANGLE_LIMIT, 1000000000);
        importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 4);
        importer.SetIOHandler(new AssImpIOSystem(file));
        scene = importer.ReadFile(file->name(), flags);
        if (!scene) {
            B3D_LOGE("Unable to load mesh \"" << file->name() << "\": " << importer.GetErrorString());
            return mesh;
        }

        if (loadSkeleton) {
            aiMatrix4x4 globalInvTransform = scene->mRootNode->mTransformation;
            globalInvTransform.Inverse();
            glm::mat4 globalInverseTransform(
                globalInvTransform.a1, globalInvTransform.b1, globalInvTransform.c1, globalInvTransform.d1,
                globalInvTransform.a2, globalInvTransform.b2, globalInvTransform.c2, globalInvTransform.d2,
                globalInvTransform.a3, globalInvTransform.b3, globalInvTransform.c3, globalInvTransform.d3,
                globalInvTransform.a4, globalInvTransform.b4, globalInvTransform.c4, globalInvTransform.d4
            );

            /*
            SkeletonPtr skeleton = std::make_shared<Skeleton>(globalInverseTransform);
            readNodeHierarchy(skeleton, scene->mRootNode, SkeletonBone::INVALID_INDEX);
            mesh->setSkeleton(skeleton);
            //B3D_LOGI(" - Total " << skeleton->numBones() << " bone" << (skeleton->numBones() == 1 ? "" : "s") << '.');

            if (skeleton->numBones() > 255) {
                B3D_LOGW("In \"" << file->name() <<
                    "\": skeleton has more than 255 bones. Probably there will be glitches in animations.");
            }
            */
        }

        BoundingBox meshBoundingBox;
        bool firstElement = true;

        aiString materialName;
        for (size_t meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++) {
            const aiMesh* sceneMesh = scene->mMeshes[meshIndex];

            if (sceneMesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE) {
                if (sceneMesh->mName.length == 0)
                    B3D_LOGW("In \"" << file->name() << "\": ignoring non-triangulated mesh #" << meshIndex << ".");
                else {
                    B3D_LOGW("In \"" << file->name() << "\": ignoring non-triangulated mesh #" << meshIndex
                        << " (\"" << sceneMesh->mName.C_Str() << "\").");
                }
                continue;
            }

            if (sceneMesh->mNumVertices > MAX_VERTICES_PER_ELEMENT) {
                if (sceneMesh->mName.length == 0)
                    B3D_LOGW("In \"" << file->name() << "\": mesh #" << meshIndex << " has too many vertices.");
                else {
                    B3D_LOGW("In \"" << file->name() << "\": mesh #" << meshIndex
                        << " (\"" << sceneMesh->mName.C_Str() << "\") has too many vertices.");
                }
                continue;
            }

            auto element = mesh->addElement<Vertex>(PrimitiveType::Triangles);
            element->setName(std::string(sceneMesh->mName.data, sceneMesh->mName.length));

            const aiMaterial* sceneMeshMaterial = scene->mMaterials[sceneMesh->mMaterialIndex];
            if (sceneMeshMaterial->Get(AI_MATKEY_NAME, materialName) == AI_SUCCESS) {
                std::string name(materialName.data, materialName.length);

                if (StringUtils::endsWith(name, "-material"))
                    name = name.substr(0, name.size() - 9);
                if (!StringUtils::endsWith(name, Material::DEFAULT_FILE_EXTENSION))
                    name += Material::DEFAULT_FILE_EXTENSION;

                std::string path = FileUtils::makeFullPath(name, file->name());
                if (!Services::fileSystem()->fileExists(path))
                    path = std::move(name);

                element->setMaterialName(std::move(path));
            }

            const bool hasPositions = sceneMesh->HasPositions();
            const bool hasNormals = sceneMesh->HasNormals();
            const bool hasTangents = sceneMesh->HasTangentsAndBitangents();
            const bool hasTexCoords = sceneMesh->HasTextureCoords(0);
            //const bool hasBones = loadSkeleton && sceneMesh->HasBones();

            size_t vertexCount = sceneMesh->mNumVertices;
//            if (!hasBones)
                Vertex* vertices = element->allocVertexBuffer(vertexCount);
//            else
//                vertexBufferOffset = mesh->appendVertices(vertexCount, &vertices, &skinningVertices);

            BoundingBox elementBoundingBox;
            if (vertexCount > 0 && hasPositions) {
                elementBoundingBox.initFromPoint(glm::vec3(
                    sceneMesh->mVertices[0].x,
                    sceneMesh->mVertices[0].y,
                    sceneMesh->mVertices[0].z
                ));
            }

            for (size_t i = 0; i < vertexCount; i++) {
                if (hasPositions) {
                    vertices->position.x = sceneMesh->mVertices[i].x;
                    vertices->position.y = sceneMesh->mVertices[i].y;
                    vertices->position.z = sceneMesh->mVertices[i].z;
                    elementBoundingBox.addPoint(vertices->position);
                }

                if (hasNormals) {
                    vertices->normal.x = sceneMesh->mNormals[i].x;
                    vertices->normal.y = sceneMesh->mNormals[i].y;
                    vertices->normal.z = sceneMesh->mNormals[i].z;
                }

                if (hasTangents) {
                    vertices->tangent.x = sceneMesh->mTangents[i].x;
                    vertices->tangent.y = sceneMesh->mTangents[i].y;
                    vertices->tangent.z = sceneMesh->mTangents[i].z;

                    vertices->bitangent.x = sceneMesh->mBitangents[i].x;
                    vertices->bitangent.y = sceneMesh->mBitangents[i].y;
                    vertices->bitangent.z = sceneMesh->mBitangents[i].z;
                }

                if (hasTexCoords) {
                    vertices->texCoord.x = sceneMesh->mTextureCoords[0][i].x;
                    vertices->texCoord.y = sceneMesh->mTextureCoords[0][i].y;
                }

                ++vertices;
            }

            element->setBoundingBox(elementBoundingBox);
            if (vertexCount > 0 && hasPositions) {
                if (!firstElement)
                    meshBoundingBox.addBoundingBox(elementBoundingBox);
                else {
                    meshBoundingBox = elementBoundingBox;
                    firstElement = false;
                }
            }

            /*
            if (hasBones) {
                for (size_t boneIndex = 0; boneIndex < sceneMesh->mNumBones; boneIndex++) {
                    const aiBone* sceneMeshBone = sceneMesh->mBones[boneIndex];

                    std::string nameString(sceneMeshBone->mName.data, sceneMeshBone->mName.length);
                    Atom boneName = Skeleton::boneNameTable()->getAtom(nameString);

                    ISkeletonBone* bone = const_cast<ISkeletonBone*>(mesh->skeleton()->bone(boneName));
                    if (!bone) {
                        B3D_LOGW("In \"" << file->name() << "\": undefined bone \"" << nameString << "\".");
                        continue;
                    }

                    const auto& m = sceneMeshBone->mOffsetMatrix;
                    static_cast<SkeletonBone*>(bone)->setMatrix(glm::mat4(
                        m.a1, m.b1, m.c1, m.d1,
                        m.a2, m.b2, m.c2, m.d2,
                        m.a3, m.b3, m.c3, m.d3,
                        m.a4, m.b4, m.c4, m.d4
                    ));

                    for (size_t i = 0; i < sceneMeshBone->mNumWeights; i++) {
                        size_t vertexId = sceneMeshBone->mWeights[i].mVertexId;
                        for (int index = 0; index < 4; index++) {
                            if (skinningVertices[vertexId].boneWeights[index] == 0.0f) {
                                skinningVertices[vertexId].boneWeights[index] = sceneMeshBone->mWeights[i].mWeight;
                                skinningVertices[vertexId].boneIndices[index] = uint8_t(bone->index());
                                break;
                            }
                        }
                    }
                }
            }
            */

            size_t indexCount = 0;
            for (size_t i = 0; i < sceneMesh->mNumFaces; i++) {
                if (sceneMesh->mFaces[i].mNumIndices != 3)
                    continue;
                indexCount += 3;
            }

            uint16_t* indices = element->allocIndexBuffer(indexCount);
            for (size_t i = 0; i < sceneMesh->mNumFaces; i++) {
                if (sceneMesh->mFaces[i].mNumIndices != 3)
                    continue;
                *indices++ = uint16_t(sceneMesh->mFaces[i].mIndices[0]);
                *indices++ = uint16_t(sceneMesh->mFaces[i].mIndices[1]);
                *indices++ = uint16_t(sceneMesh->mFaces[i].mIndices[2]);
            }
        }

        mesh->setBoundingBox(meshBoundingBox);

        return mesh;
    }
}
