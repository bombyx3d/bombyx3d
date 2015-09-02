
project(Engine)

add_subdirectory(platform)

set(source_files
    core/Log.cpp
    core/Log.h
    interfaces/core/IApplication.h
    interfaces/render/IRenderer.h
    render/opengl.h
    render/Renderer.cpp
    render/Renderer.h
)

add_library(engine STATIC EXCLUDE_FROM_ALL ${source_files})
set_source_groups(${source_files})

set_target_properties(engine PROPERTIES CXX_STANDARD 11)
target_link_libraries(engine platform glm)
