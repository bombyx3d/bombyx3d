#include "game/client/Application.h"

using namespace Engine;

IApplication* IApplication::create()
{
    return new Application();
}
