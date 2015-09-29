#include "AssImpMeshLoader.h"

static void init()
{
    B3D::RawMeshData::registerLoader<B3D::AssImpMeshLoader>();
}
