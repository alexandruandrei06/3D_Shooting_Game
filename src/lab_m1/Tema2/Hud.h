#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace Hud
{
    //Functie pentru crearea mesh-ului pentru rama timpului
    Mesh* CreateFrameTime(const std::string& name, float x, float y);

    //Functie pentru crearea mesh-ului utilizat la umplerea timpului
    Mesh* CreateFillTime(const std::string& name, float x, float y);

    //Functie pentru crearea mesh-ului pentru rama healtbar-ului
    Mesh* CreateFrameLife(const std::string& name, float x, float y);

    //Functie pentru crearea mesh-ului utilizat la umplerea healtbar-ului
    Mesh* CreateFillLife(const std::string& name, float x, float y);
}