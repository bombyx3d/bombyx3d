#include "Application.h"
#include "engine/interfaces/render/IRenderer.h"
#include "engine/core/Log.h"
#include "engine/core/ResourceManager.h"
#include "engine/utility/RenderUtils.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    Application* Application::mInstance;

    Application::Application()
    {
        assert(mInstance == nullptr);
        mInstance = this;
    }

    Application::~Application()
    {
        assert(mInstance == this);
        mInstance = nullptr;
    }

    glm::ivec2 Application::preferredScreenSize() const
    {
        return glm::ivec2(1024, 768);
    }

    int Application::preferredDepthBits() const
    {
        return 24;
    }

    int Application::preferredStencilBits() const
    {
        return 8;
    }

    // FIXME: test code
    static ShaderPtr gShader;
    static VertexBufferPtr gVertexBuffer;
    static VertexSourcePtr gVertexSource;

    Z_VERTEX_FORMAT(MyVertexFormat,
        (glm::vec3) position,
        (glm::vec4) color
    )

    void Application::initialize(const glm::ivec2& screenSize)
    {
        mScreenSize = screenSize;

        gShader = ResourceManager::instance()->getShader("shaders/BasicPerVertexColored.glsl", true);

        gVertexBuffer = IRenderer::instance()->createVertexBuffer();
        static const MyVertexFormat vertices[] = {
            { { -0.5f, -0.5f, -2.0f, }, { 1.0f, 0.0f, 0.0f, 1.0f }, },
            { {  0.5f, -0.5f, -2.0f, }, { 0.0f, 1.0f, 0.0f, 1.0f }, },
            { { -0.5f,  0.5f, -2.0f, }, { 0.0f, 0.0f, 0.0f, 1.0f, }, },
            { {  0.5f,  0.5f, -2.0f, }, { 1.0f, 1.0f, 0.0f, 1.0f, }, },
        };
        gVertexBuffer->setData(vertices, sizeof(vertices), BufferUsage::Static);

        gVertexSource = RenderUtils::createVertexSource<MyVertexFormat>(gVertexBuffer);
    }

    void Application::shutdown()
    {
        gShader.reset();
        gVertexBuffer.reset();
        gVertexSource.reset();
    }

    void Application::resize(const glm::ivec2& screenSize)
    {
        mScreenSize = screenSize;
    }

    void Application::runFrame(double)
    {
        IRenderer::instance()->beginFrame();
        IRenderer::instance()->setViewport(0, 0, mScreenSize.x, mScreenSize.y);
        IRenderer::instance()->clear();

        float aspect = float(mScreenSize.x) / float(mScreenSize.y);
        glm::mat4 proj = glm::perspective(glm::radians(90.0f), aspect, 1.0f, 100.0f);
        glm::mat4 mv = glm::mat4(1.0f);

        IRenderer::instance()->useShader(gShader);
        IRenderer::instance()->bindVertexSource(gVertexSource);

        IRenderer::instance()->setUniform(AtomTable::instance()->getAtom("uProjection"), proj);
        IRenderer::instance()->setUniform(AtomTable::instance()->getAtom("uModelView"), mv);
        IRenderer::instance()->drawPrimitive(PrimitiveType::TriangleStrip, 0, 4);

        IRenderer::instance()->endFrame();
    }
}
