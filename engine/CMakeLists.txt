
project(Engine)

add_subdirectory(platform)

set(source_files
    core/Log.cpp
    core/Log.h
    core/macros.h
    interfaces/core/IApplication.h
    interfaces/io/IFile.h
    interfaces/io/IFileSystem.h
    interfaces/render/IRenderer.h
    interfaces/render/IShader.h
    interfaces/render/ITexture.h
    render/opengl.h
    render/Renderer.cpp
    render/Renderer.h
    render/Shader.cpp
    render/Shader.h
    render/Texture.cpp
    render/Texture.h
    utility/FileUtils.cpp
    utility/FileUtils.h
    utility/Singleton.h
)

add_library(engine STATIC EXCLUDE_FROM_ALL ${source_files})
set_source_groups(${source_files})

set_target_properties(engine PROPERTIES CXX_STANDARD 11)
target_link_libraries(engine platform glm)
