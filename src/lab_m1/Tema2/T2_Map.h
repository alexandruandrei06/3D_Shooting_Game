#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace T2_Map
{
    //Functie pentru crearea mesh-ului pentru o celula a grid-ului pentru harta
    Mesh* CreateMapCell(const std::string& name);

    //Functie pentru crearea mesh-ului pentru peretele asociat unei celule din grid-ului hartii
    Mesh* CreateMapWall(const std::string& name);
}