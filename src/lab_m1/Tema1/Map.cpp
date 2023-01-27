#include "Map.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

//Define-uri cu vectorii pentru culorile hartii si a obstacolelor
#define mapColor glm::vec3(0.788f, 0.780f, 0.819f)
#define mapMarginColor glm::vec3(0.874, 0.066, 0.078)
#define obsColor glm::vec3(1, 0.501, 0.058)
#define marginWidth 0.35f


Mesh* Map::CreateMap(
    const std::string& name, int x, int y)
{
    std::vector<VertexFormat> vertices =
    {
        //Harta 
        VertexFormat(glm::vec3(0, 0, 0), mapColor),    //0
        VertexFormat(glm::vec3(0, y, 0), mapColor),    //1
        VertexFormat(glm::vec3(x, y, 0), mapColor),    //2
        VertexFormat(glm::vec3(x, 0, 0), mapColor),    //3

        //Chenar Harta
         VertexFormat(glm::vec3(-marginWidth, -marginWidth, 0), mapMarginColor),    //4
         VertexFormat(glm::vec3(-marginWidth, marginWidth + y, 0), mapMarginColor), //5
         VertexFormat(glm::vec3(0, marginWidth + y, 0), mapMarginColor),            //6
         VertexFormat(glm::vec3(0, -marginWidth, 0), mapMarginColor),               //7

         VertexFormat(glm::vec3(x, -marginWidth, 0), mapMarginColor),                   //8
         VertexFormat(glm::vec3(x, marginWidth + y, 0), mapMarginColor),                //9
         VertexFormat(glm::vec3(x + marginWidth, marginWidth + y, 0), mapMarginColor),  //10
         VertexFormat(glm::vec3(x + marginWidth, -marginWidth, 0), mapMarginColor),     //11

         VertexFormat(glm::vec3(0, 0, 0), mapMarginColor),            //12
         VertexFormat(glm::vec3(x, 0, 0), mapMarginColor),            //13
         VertexFormat(glm::vec3(x, -marginWidth, 0), mapMarginColor), //14
         VertexFormat(glm::vec3(0, -marginWidth, 0), mapMarginColor), //15

         VertexFormat(glm::vec3(0, y + marginWidth, 0), mapMarginColor), //16
         VertexFormat(glm::vec3(x, y + marginWidth, 0), mapMarginColor), //17
         VertexFormat(glm::vec3(x, y, 0), mapMarginColor),               //18
         VertexFormat(glm::vec3(0, y, 0), mapMarginColor),               //19
    };

    Mesh* map = new Mesh(name);
    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7,
        8, 9, 10,
        8, 10, 11,
        12, 13, 14,
        12, 14, 15,
        16, 17, 18,
        16, 18, 19,
    };

    map->InitFromData(vertices, indices);
    return map;
}

Mesh* Map::CreateObsDrept(const std::string& name, int x, int y)
{
    std::vector<VertexFormat> vertices =
    {
        //Obstacol drept
        VertexFormat(glm::vec3(0, 0, 0), obsColor),    //0
        VertexFormat(glm::vec3(0, y, 0), obsColor),    //1
        VertexFormat(glm::vec3(x, y, 0), obsColor),    //2
        VertexFormat(glm::vec3(x, 0, 0), obsColor),    //3
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
