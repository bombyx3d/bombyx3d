#include "XmlSpriteSheetLoader.h"

static void init()
{
    B3D::SpriteSheet::registerLoader<B3D::XmlSpriteSheetLoader>();
}
