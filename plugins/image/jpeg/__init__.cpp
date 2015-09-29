#include "JpegImageLoader.h"

static void init()
{
    Engine::Image::registerLoader<Engine::JpegImageLoader>();
}
