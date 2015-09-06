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
    interfaces/core/IApplication.h
    interfaces/core/IThreadManager.h
    interfaces/io/IFile.h
    interfaces/io/IFileSystem.h
    interfaces/render/IBuffer.h
    interfaces/render/IIndexBuffer.h
    interfaces/render/IRenderer.h
    interfaces/render/IShader.h
    interfaces/render/ITexture.h
    interfaces/render/IVertexBuffer.h
    interfaces/render/IVertexSource.h
    interfaces/scene/IScene.h
    material/ShaderLoader.cpp
    material/ShaderLoader.h
    mesh/VertexFormat.h
    render/Buffer.cpp
    render/Buffer.h
    render/opengl.h
    render/Renderer.cpp
    render/Renderer.h
    render/Shader.cpp
    render/Shader.h
    render/Texture.cpp
    render/Texture.h
    render/Uniform.cpp
    render/Uniform.h
    render/VertexSource.cpp
    render/VertexSource.h
    scene/AbstractScene.cpp
    scene/AbstractScene.h
    utility/FileUtils.cpp
    utility/FileUtils.h
    utility/MemoryPool.cpp
    utility/MemoryPool.h
    utility/PoolAllocator.h
    utility/ProducerConsumerQueue.cpp
    utility/ProducerConsumerQueue.h
    utility/RenderUtils.h
    utility/Singleton.h
    utility/WorkerThread.cpp
    utility/WorkerThread.h
)

add_library(engine STATIC EXCLUDE_FROM_ALL ${source_files})
set_source_groups(${source_files})

set_target_properties(engine PROPERTIES CXX_STANDARD 11)
target_link_libraries(engine platform glm boost)
