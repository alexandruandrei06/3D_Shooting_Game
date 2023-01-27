#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace T2_Player
{
    //Functie pentru crearea mesh-ului din care este format jucatorul
    Mesh* CreatePlayerPart(const std::string& name, int part);
}