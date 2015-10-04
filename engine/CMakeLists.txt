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

add_subdirectory(platform)

set(source_files
    core/Application.cpp
    core/Application.h
    core/Atom.h
    core/AtomTable.cpp
    core/AtomTable.h
    core/Event.h
    core/EventDispatcher.cpp
    core/EventDispatcher.h
    core/Log.cpp
    core/Log.h
    core/macros.h
    core/ResourceManager.cpp
    core/ResourceManager.h
    core/Services.cpp
    core/Services.h
    image/Image.cpp
    image/Image.h
    image/Sprite.cpp
    image/Sprite.h
    image/SpriteSheet.cpp
    image/SpriteSheet.h
    input/InputManager.cpp
    input/InputManager.h
    input/MouseButton.h
    input/TouchEvent.h
    interfaces/core/IApplication.h
    interfaces/core/IEvent.h
    interfaces/core/IEventDispatcher.h
    interfaces/core/IEventObserver.h
    interfaces/core/ILogger.h
    interfaces/core/IResourceManager.h
    interfaces/core/IThreadManager.h
    interfaces/image/IImage.h
    interfaces/image/IImageLoader.h
    interfaces/image/ISprite.h
    interfaces/image/ISpriteSheet.h
    interfaces/image/ISpriteSheetLoader.h
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
    interfaces/render/lowlevel/IBuffer.h
    interfaces/render/lowlevel/IIndexBuffer.h
    interfaces/render/lowlevel/IRenderer.h
    interfaces/render/lowlevel/IRendererResourceFactory.h
    interfaces/render/lowlevel/IShader.h
    interfaces/render/lowlevel/ITexture.h
    interfaces/render/lowlevel/IVertexBuffer.h
    interfaces/render/lowlevel/IVertexSource.h
    interfaces/render/ICanvas.h
    interfaces/render/IImmediateModeRenderer.h
    interfaces/scene/ICamera.h
    interfaces/scene/IScene.h
    interfaces/scene/ISceneComponent.h
    interfaces/scene/ISceneManager.h
    material/Material.cpp
    material/Material.h
    material/MaterialPass.cpp
    material/MaterialPass.h
    material/MaterialTechnique.cpp
    material/MaterialTechnique.h
    material/ShaderLoader.cpp
    material/ShaderLoader.h
    math/AffineTransform.h
    math/AffineTransform.cpp
    math/AspectRatio.h
    math/Axis.h
    math/Direction.h
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
    scene/camera/AbstractCamera.cpp
    scene/camera/AbstractCamera.h
    scene/camera/AbstractPerspectiveCamera.cpp
    scene/camera/AbstractPerspectiveCamera.h
    scene/camera/OrbitCamera.cpp
    scene/camera/OrbitCamera.h
    scene/camera/OrthogonalCamera.cpp
    scene/camera/OrthogonalCamera.h
    scene/components/ChildrenListComponent.cpp
    scene/components/ChildrenListComponent.h
    scene/AbstractLoadingScene.cpp
    scene/AbstractLoadingScene.h
    scene/AbstractScene.cpp
    scene/AbstractScene.h
    scene/AbstractSceneComponent.cpp
    scene/AbstractSceneComponent.h
    scene/SceneManager.cpp
    scene/SceneManager.h
    ui/UIElement.cpp
    ui/UIElement.h
    ui/UIProgressBar.cpp
    ui/UIProgressBar.h
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
    utility/TypeID.h
    utility/WorkerThread.cpp
    utility/WorkerThread.h
)

add_library(bombyx3d-core STATIC EXCLUDE_FROM_ALL ${source_files})
b3d_set_source_groups(${source_files})

set_target_properties(bombyx3d-core PROPERTIES CXX_STANDARD 11 FOLDER "Bombyx3D")

target_link_libraries(bombyx3d-core bombyx3d-platform ${CMAKE_THREAD_LIBS_INIT})
b3d_target_link_libraries(bombyx3d-core glm boost)
if(B3D_WIN32 OR B3D_WINRT)
    b3d_target_link_libraries(bombyx3d-core angle)
endif()

if(MSVC)
    target_compile_definitions(bombyx3d-core PUBLIC
        _CRT_SECURE_NO_WARNINGS
        _CRT_SECURE_NO_DEPRECATE
        _CRT_NONSTDC_NO_DEPRECATE
    )
else()
    set(flags -pedantic -Wall -Wextra -Wconversion)
    if(NOT CMAKE_COMPILER_IS_GNUCC AND NOT CMAKE_COMPILER_IS_GNUCC)
        list(APPEND flags -Wshadow)
    endif()
    target_compile_options(bombyx3d-core BEFORE PUBLIC "${flags}")
endif()
