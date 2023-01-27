#include "Player.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

//Define-uri cu vectorii pentru culorile tancului si a proiectilului
#define bulletColor glm::vec3(0, 0, 0)
#define tankBodyColor glm::vec3(0.031f, 0.211f, 0.349f)
#define senileColor glm::vec3(0.29f, 0.45f, 0.89f)
#define teavaColor glm::vec3(0.145f, 0.956f, 0.882f)


Mesh* Player::CreatePlayerBody(
    const std::string& name)
{   
    std::vector<VertexFormat> vertices =
    {
        //Corpul tancului
        VertexFormat(glm::vec3(-0.5f, -1, 0), tankBodyColor), //0
        VertexFormat(glm::vec3(0.5f, -1, 0), tankBodyColor),  //1
        VertexFormat(glm::vec3(0.5f, 0, 0), tankBodyColor),  //2
        VertexFormat(glm::vec3(-0.5f, 0, 0), tankBodyColor), //3

    };

    Mesh* player = new Mesh(name);
    std::vector<unsigned int> indices = 
    { 
        0, 1, 2,
        0, 2, 3,
    };

    player->InitFromData(vertices, indices);
    return player;

  
}

Mesh* Player::CreatePlayerSenile(const std::string& name)
{
    std::vector<VertexFormat> vertices =
    {
        //Senila stanga a tancului
        VertexFormat(glm::vec3(-1, -1, 0), senileColor),    //0
        VertexFormat(glm::vec3(-1, 1, 0), senileColor),    //1
        VertexFormat(glm::vec3(-0.5f, 1, 0), senileColor), //2
        VertexFormat(glm::vec3(-0.5f, -1, 0), senileColor), //3

        //Senila dreapta a tancului
        VertexFormat(glm::vec3(0.5f, -1, 0), senileColor),  //4
        VertexFormat(glm::vec3(0.5f, 1, 0), senileColor),  //5
        VertexFormat(glm::vec3(1, 1, 0), senileColor),     //6
        VertexFormat(glm::vec3(1, -1, 0), senileColor),     //7
    };

    Mesh* map = new Mesh(name);
    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7,
    };

    map->InitFromData(vertices, indices);
    return map;
}

Mesh* Player::CreatePlayerWeapon(const std::string& name)
{
    std::vector<VertexFormat> vertices =
    {
        //Teava tancului
        VertexFormat(glm::vec3(-0.15f, 0, 0), teavaColor),   //0
        VertexFormat(glm::vec3(-0.15f, 1.5f, 0), teavaColor), //1
        VertexFormat(glm::vec3(0.15f, 1.5f, 0), teavaColor), //2
        VertexFormat(glm::vec3(0.15f, 0, 0), teavaColor),    //3
    };

    Mesh* map = new Mesh(name);
    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3,
    };

    map->InitFromData(vertices, indices);
    return map;
}

Mesh* Player::CreateBullet(const std::string& name, int x, int y)
{
    std::vector<VertexFormat> vertices =
    {
        //Proiectil in forma de dreptunchi
        VertexFormat(glm::vec3(0, 0, 0), bulletColor),    //0
        VertexFormat(glm::vec3(0, y, 0), bulletColor),    //1
        VertexFormat(glm::vec3(x, y, 0), bulletColor),    //2
        VertexFormat(glm::vec3(x, 0, 0), bulletColor),    //3
    };

    Mesh* map = new Mesh(name);
    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3,
    };

    map->InitFromData(vertices, indices);
    return map;
}
