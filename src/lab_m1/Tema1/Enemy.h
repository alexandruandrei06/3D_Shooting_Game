#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace Enemy
{
    //Functie pentru crearea mesh-ului pentru corpul inamicului
    Mesh* CreateEnemyBody(const std::string& name);

    //Functie pentru crearea mesh-ului pentru bratele inamicului
    Mesh* CreateEnemyArms(const std::string& name);
}