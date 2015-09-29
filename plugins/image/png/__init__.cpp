#include "PngImageLoader.h"

static void init()
{
    B3D::Image::registerLoader<B3D::PngImageLoader>();
}
