
project(Engine)

add_subdirectory(platform)

set(source_files
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
