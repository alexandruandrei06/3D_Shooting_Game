#include "T2_Map.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

//Define-uri cu vectorii pentru culorile hartii
#define cellColor glm::vec3(0.560, 0.701, 0.960)
#define wallColor glm::vec3(0.027, 0.050, 0.513)

Mesh* T2_Map::CreateMapCell(const std::string& name)
{
    std::vector<VertexFormat> vertices =
    {
        //Grid
        VertexFormat(glm::vec3(0, 0, 0), cellColor),    //0
        VertexFormat(glm::vec3(0, 0, 1), cellColor),    //1
        VertexFormat(glm::vec3(1, 0, 1), cellColor),    //2
        VertexFormat(glm::vec3(1, 0, 0), cellColor),    //3
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3
    };

    Mesh* map_grid = new Mesh(name);

    map_grid->InitFromData(vertices, indices);
    return map_grid;
}


Mesh* T2_Map::CreateMapWall(const std::string& name)
{
    std::vector<VertexFormat> vertices
    {
        //Peretele (cub cu latura de 1, scalat dupa)
        VertexFormat(glm::vec3(0, 0, 1), wallColor),
        VertexFormat(glm::vec3(1, 0, 1), wallColor),
        VertexFormat(glm::vec3(0, 1, 1), wallColor),
        VertexFormat(glm::vec3(1, 1, 1), wallColor),
        VertexFormat(glm::vec3(0, 0, 0), wallColor),
        VertexFormat(glm::vec3(1, 0, 0), wallColor),
        VertexFormat(glm::vec3(0, 1, 0), wallColor),
        VertexFormat(glm::vec3(1, 1, 0), wallColor),
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
