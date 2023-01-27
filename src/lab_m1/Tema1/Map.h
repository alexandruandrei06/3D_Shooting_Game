#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace Map
{
    /*Functie pentru crearea mesh-ului pentru harta, care cuprinde harta 
        propriu-zisa si un chenar format din 4 dreptunchiuri*/
    Mesh* CreateMap(const std::string& name, int x, int y);

    //Functie pentru crearea mesh-ului petru obstacole 
    Mesh* CreateObsDrept(const std::string& name, int x, int y);
}