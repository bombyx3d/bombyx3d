#include "XmlSpriteSheetLoader.h"

static void init()
{
    Engine::SpriteSheet::registerLoader<Engine::XmlSpriteSheetLoader>();
}
