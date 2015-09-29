#include "PngImageLoader.h"

static void init()
{
    Engine::Image::registerLoader<Engine::PngImageLoader>();
}
