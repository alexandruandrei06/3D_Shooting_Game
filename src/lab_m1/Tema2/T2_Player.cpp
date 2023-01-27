#include "T2_Player.h"

#include <vector>
#include <iostream>
#include "core/engine.h"
#include "utils/gl_utils.h"

//Define-uri cu vectorii pentru culorile partilor jucatorului
#define headColor glm::vec3(0.988, 0.815, 0.533)
#define body_armColor glm::vec3(0.772, 0.172, 0.027)
#define legsColor glm::vec3(0.176, 0.431, 0.168)

#define HEAD 1
#define BODY_ARM 2
#define ARM 3
#define LEGS 4

/*In functie de partea corpului(int part), mesh-ul va fi colorat diferit*/

Mesh* T2_Player::CreatePlayerPart(const std::string& name, int part)
{
    glm::vec3 color;
    switch(part){
        case HEAD: {
            color = headColor;
            break;
        }
        case BODY_ARM: {
            color = body_armColor;
            break;
        }
        case ARM: {
            color = headColor;
            break;
        }
        case LEGS: {
            color = legsColor;
            break;
        }
        default: {
            std::cout << "PULA MEA\n";
            break;

        }
            break;
    }
    
    std::vector<VertexFormat> vertices
    {
        
        VertexFormat(glm::vec3(-0.5, -0.5, 0.5), color),
        VertexFormat(glm::vec3(0.5, -0.5, 0.5), color),
        VertexFormat(glm::vec3(-0.5, 0.5, 0.5), color),
        VertexFormat(glm::vec3(0.5, 0.5, 0.5), color),
        VertexFormat(glm::vec3(-0.5, -0.5, -0.5), color),
        VertexFormat(glm::vec3(0.5, -0.5, -0.5), color),
        VertexFormat(glm::vec3(-0.5, 0.5, -0.5), color),
        VertexFormat(glm::vec3(0.5, 0.5, -0.5), color),
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        1, 3, 2,
        2, 3, 7,
        2, 7, 6,
        1, 7, 3,
        1, 5, 7,
        6, 7, 4,
        7, 5, 4,
        0, 4, 1,
        1, 4, 5,
        2, 6, 4,
        0, 2, 4,
    };



    Mesh* map_wall = new Mesh(name);

    map_wall->InitFromData(vertices, indices);
    return map_wall;
}
