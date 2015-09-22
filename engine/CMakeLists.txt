#
# Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com).
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

project(Engine)

add_subdirectory(platform)

set(source_files
    core/Application.cpp
    core/Application.h
    core/Atom.h
    core/AtomTable.cpp
    core/AtomTable.h
    core/Log.cpp
    core/Log.h
    core/macros.h
    core/ResourceManager.cpp
    core/ResourceManager.h
    core/Services.cpp
    core/Services.h
    image/Image.cpp
    image/Image.h
    input/InputManager.cpp
    input/InputManager.h
    input/MouseButton.h
    interfaces/core/IApplication.h
    interfaces/core/ILogger.h
    interfaces/core/IResourceManager.h
    interfaces/core/IThreadManager.h
    interfaces/image/IImage.h
    interfaces/image/IImageLoader.h
    interfaces/input/IInputManager.h
    interfaces/input/IInputObserver.h
    interfaces/io/IFile.h
    interfaces/io/IFileSystem.h
    interfaces/material/IMaterial.h
    interfaces/material/IMaterialLoader.h
    interfaces/material/IMaterialPass.h
    interfaces/material/IMaterialTechnique.h
    interfaces/mesh/IMesh.h
    interfaces/mesh/IMeshLoader.h
    interfaces/mesh/IRawMeshData.h
    interfaces/mesh/IRawMeshElementData.h
    interfaces/render/IBuffer.h
    interfaces/render/IIndexBuffer.h
    interfaces/render/IRenderer.h
    interfaces/render/IShader.h
    interfaces/render/ISprite.h
    interfaces/render/ITexture.h
    interfaces/render/IVertexBuffer.h
    interfaces/render/IVertexSource.h
    interfaces/scene/3d/camera/ICamera.h
    interfaces/scene/IScene.h
    material/Material.cpp
    material/Material.h
    material/MaterialPass.cpp
    material/MaterialPass.h
    material/MaterialTechnique.cpp
    material/MaterialTechnique.h
    material/ShaderLoader.cpp
    material/ShaderLoader.h
    math/Axis.h
    math/BoundingBox.cpp
    math/BoundingBox.h
    math/Quad.h
    mesh/Mesh.cpp
    mesh/Mesh.h
    mesh/RawMeshData.cpp
    mesh/RawMeshData.h
    mesh/RawMeshElementData.h
    mesh/VertexFormat.h
    render/gles2/GLES2Buffer.cpp
    render/gles2/GLES2Buffer.h
    render/gles2/GLES2Renderer.cpp
    render/gles2/GLES2Renderer.h
    render/gles2/GLES2Shader.cpp
    render/gles2/GLES2Shader.h
    render/gles2/GLES2Texture.cpp
    render/gles2/GLES2Texture.h
    render/gles2/GLES2Uniform.cpp
    render/gles2/GLES2Uniform.h
    render/gles2/GLES2VertexSource.cpp
    render/gles2/GLES2VertexSource.h
    render/gles2/opengl.cpp
    render/gles2/opengl.h
    render/Canvas.cpp
    render/Canvas.h
    render/ImmediateModeRenderer.cpp
    render/ImmediateModeRenderer.h
    render/Sprite.cpp
    render/Sprite.h
    scene/3d/camera/AbstractCamera.cpp
    scene/3d/camera/AbstractCamera.h
    scene/3d/camera/AbstractOrthogonalCamera.cpp
    scene/3d/camera/AbstractOrthogonalCamera.h
    scene/3d/camera/AbstractPerspectiveCamera.cpp
    scene/3d/camera/AbstractPerspectiveCamera.h
    scene/3d/camera/OrbitCamera.cpp
    scene/3d/camera/OrbitCamera.h
    scene/AbstractLoadingScene.cpp
    scene/AbstractLoadingScene.h
    scene/AbstractScene.cpp
    scene/AbstractScene.h
    utility/FileUtils.cpp
    utility/FileUtils.h
    utility/MemoryPool.cpp
    utility/MemoryPool.h
    utility/ObserverList.h
    utility/PoolAllocator.h
    utility/ProducerConsumerQueue.cpp
    utility/ProducerConsumerQueue.h
    utility/RenderUtils.h
    utility/ScopedCounter.h
    utility/StringUtils.cpp
    utility/StringUtils.h
    utility/WorkerThread.cpp
    utility/WorkerThread.h
)

add_library(engine STATIC EXCLUDE_FROM_ALL ${source_files})
z_set_source_groups(${source_files})

set_target_properties(engine PROPERTIES CXX_STANDARD 11)
target_link_libraries(engine platform glm boost)

add_subdirectory(image/loaders/png)
add_subdirectory(material/loaders/text)
add_subdirectory(mesh/loaders/assimp)
