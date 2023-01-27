#include "Enemy.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

//Define-uri cu vectorii pentru culorile inamicului
#define handColor glm::vec3(0.164f, 0.956f, 0.9257f)
#define bodyColor glm::vec3(1, 0, 0)


Mesh* Enemy::CreateEnemyBody(
    const std::string& name)
{
    std::vector<VertexFormat> vertices =
    {
        //Corpul inamicului
        VertexFormat(glm::vec3(-1, -1, 0), glm::vec3(bodyColor)),//0
        VertexFormat(glm::vec3(-1, 1, 0), glm::vec3(bodyColor)), //1
        VertexFormat(glm::vec3(1, 1, 0), glm::vec3(bodyColor)),  //2
        VertexFormat(glm::vec3(1, -1, 0), glm::vec3(bodyColor)), //3

    };

    Mesh* enemy = new Mesh(name);
    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3,
    };

    enemy->InitFromData(vertices, indices);
    return enemy;
}

Mesh* Enemy::CreateEnemyArms(
    const std::string& name)
{
    std::vector<VertexFormat> vertices =
    {
        //bratul stang inamic
        VertexFormat(glm::vec3(-1, 1, 0), glm::vec3(handColor)),    //0
        VertexFormat(glm::vec3(-0.5f, 2, 0), glm::vec3(handColor)), //1
        VertexFormat(glm::vec3(-0.5f, 1, 0), glm::vec3(handColor)), //2

        //bratul drept inamic
        VertexFormat(glm::vec3(0.5, 1, 0), glm::vec3(handColor)),  //3
        VertexFormat(glm::vec3(0.5, 2, 0), glm::vec3(handColor)),  //4
        VertexFormat(glm::vec3(1, 1, 0), glm::vec3(handColor)),    //5

    };

    Mesh* enemy = new Mesh(name);
    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        3, 4, 5,
    };

    enemy->InitFromData(vertices, indices);
    return enemy;
}