#include "TextMaterialLoader.h"

static void init()
{
    Engine::Material::registerLoader<Engine::TextMaterialLoader>();
}
