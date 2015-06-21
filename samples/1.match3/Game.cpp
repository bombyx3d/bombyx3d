#include "Game.h"
#include "core/interfaces/ISystem.h"
#include "core/interfaces/IViewport.h"
#include "_OLD/Engine.h"
#include "_OLD/canvas/Canvas.h"
#include "_OLD/canvas/CanvasSprite.h"
#include "_OLD/mechanics/match3/Match3Field.h"
#include "_OLD/mechanics/match3/Match3View.h"
#include "_OLD/renderer/opengl/api/opengl.h"
#include "system/main.h"
#include <cstdio>

Z_GAME_CLASS(Game)

const int WIDTH = 8;
const int HEIGHT = 8;

Z::Match3FieldPtr field;
Z::Match3ViewPtr view;
Z::SpritePtr elementSprite[6];
Z::CanvasPtr canvas;

namespace
{
    class Factory : public Z::Match3SpriteFactory
    {
    public:
        Z::SpritePtr spriteForElement(uint8_t index) override
        {
            return elementSprite[index];
        }
    };

    static Factory spriteFactory;
}

bool Game::initialize()
{
    canvas = std::make_shared<Z::Canvas>();

    elementSprite[0] = Z::Engine::instance().renderer().createSpriteFromTexture("data/Croissant.png");
    elementSprite[1] = Z::Engine::instance().renderer().createSpriteFromTexture("data/Macaroon.png");
    elementSprite[2] = Z::Engine::instance().renderer().createSpriteFromTexture("data/SugarCookie.png");
    elementSprite[3] = Z::Engine::instance().renderer().createSpriteFromTexture("data/Cupcake.png");
    elementSprite[4] = Z::Engine::instance().renderer().createSpriteFromTexture("data/Danish.png");
    elementSprite[5] = Z::Engine::instance().renderer().createSpriteFromTexture("data/Donut.png");

    field = std::make_shared<Z::Match3Field>(WIDTH, HEIGHT, 6);

    view = std::make_shared<Z::Match3View>(&spriteFactory);
    view->setField(field);
    view->setDrawDebugBorder(true);
    view->setPosition(150.0f, 50.0f);
    view->setRotation(10 * 3.1415f / 180.0f);
    canvas->rootElement()->addChild(view);

    return true;
}

void Game::shutdown()
{
    canvas.reset();
}

void Game::runFrame(double time)
{
    static double t = 0;
    //t += time;
    if (t >= 5.0) {
        t -= 5.0;
        int x1 = 1 + rand() % (field->width() - 2);
        int y1 = 1 + rand() % (field->height() - 2);
        int x2, y2;
        //do {
            switch (rand() % 3) {
            case 0: x2 = x1 + 1; y2 = y1; break;
            case 1: x2 = x1 - 1; y2 = y1; break;
            case 2: x2 = x1; y2 = y1 + 1; break;
            case 3: x2 = x1; y2 = y1 - 1; break;
            }
        //} while (!field->swapItems(x1, y1, x2, y2));
        field->swapItems(x1, y1, x2, y2, false);
    }

    canvas->update(time);

    auto& renderer = Z::Engine::instance().renderer();
    renderer.begin2D();

    gl::Enable(GL::BLEND);
    gl::BlendFunc(GL::SRC_ALPHA, GL::ONE_MINUS_SRC_ALPHA);

    canvas->draw();

    renderer.end2D();
}

void Game::onPointerPressed(int id, const glm::vec2& pos)
{
    canvas->sendPointerPressEvent(id, pos);
}

void Game::onPointerMoved(int id, const glm::vec2& pos)
{
    canvas->sendPointerMoveEvent(id, pos);
}

void Game::onPointerReleased(int id, const glm::vec2& pos)
{
    canvas->sendPointerReleaseEvent(id, pos);
}

void Game::onPointerCancelled(int id, const glm::vec2& pos)
{
    canvas->sendPointerCancelEvent(id, pos);
}

////////////////////////////////////

#ifdef Z_TARGET_QT5
#include "system/qt5/io/QtFileSystem.h"
#endif

using namespace Z;
using namespace Engine;

int gameMain()
{
    Ptr<Z::Engine> engine = new Z::Engine;

  #if defined(Z_TARGET_QT5) && defined(Z_DEBUG_ASSETS_PATH)
    const char* assetsLocation = Z_DEBUG_ASSETS_PATH;
    if (assetsLocation)
        ICore::instance().registerFileSystem(new QtFileSystem(assetsLocation));
  #endif

    ViewportSettings viewportSettings;
    Ptr<IViewport> viewport = ISystem::instance().createViewport(viewportSettings, engine);

    ISystem::instance().runEventLoop();

    viewport.release();
    engine.release();

    return 0;
}
