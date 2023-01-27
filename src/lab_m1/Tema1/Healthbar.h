#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace Healthbar
{
    //Functie pentru crearea mesh-ului pentru rama healtbar-ului
    Mesh* CreateFrame(const std::string& name, float x, float y);

    //Functie pentru crearea mesh-ului utilizat la umplerea healtbar-ului
    Mesh* CreateFill(const std::string& name, float x, float y);
}

