#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace Player
{
    //Functie pentru crearea mesh-ului petru corpul tancului
    Mesh* CreatePlayerBody(const std::string& name);
    
    //Functie pentru crearea mesh-ului petru senilele tancului
    Mesh* CreatePlayerSenile(const std::string& name);

    //Functie pentru crearea  mesh-ului petru teava tancului
    Mesh* CreatePlayerWeapon(const std::string& name);

    //Functie pentru crearea  mesh-ului petru proiectilul tancului
    Mesh* CreateBullet(const std::string& name, int x, int y);
}