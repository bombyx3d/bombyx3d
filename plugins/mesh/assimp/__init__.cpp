#include "AssImpMeshLoader.h"

static void init()
{
    Engine::RawMeshData::registerLoader<Engine::AssImpMeshLoader>();
}
