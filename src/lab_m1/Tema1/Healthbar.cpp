#include "Healthbar.h"

#include "core/engine.h"
#include "utils/gl_utils.h"
//Define cu vectorul pentru culoarea healtbar-ului
#define color glm::vec3(0.223f, 0.925f, 0.152f)

Mesh* Healthbar::CreateFrame(
    const std::string& name, float x, float y)
{
    std::vector<VertexFormat> vertices =
    {
       
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(color)), //0
        VertexFormat(glm::vec3(0, y, 0), glm::vec3(color)), //1
        VertexFormat(glm::vec3(x, y, 0), glm::vec3(color)), //2
        VertexFormat(glm::vec3(x, 0, 0), glm::vec3(color)), //3

    };

    Mesh* frame = new Mesh(name);
    std::vector<unsigned int> indices =
    {
        0, 1, 2, 3
    };
    frame->SetDrawMode(GL_LINE_LOOP);

    frame->InitFromData(vertices, indices);
    return frame;
}

Mesh* Healthbar::CreateFill(
    const std::string& name, float x, float y)
{
    std::vector<VertexFormat> vertices =
    {

        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(color)), //0
        VertexFormat(glm::vec3(0, y, 0), glm::vec3(color)), //1
        VertexFormat(glm::vec3(x, y, 0), glm::vec3(color)), //2
        VertexFormat(glm::vec3(x, 0, 0), glm::vec3(color)), //3

    };

    Mesh* fill = new Mesh(name);
    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3,
    };

    fill->InitFromData(vertices, indices);
    return fill;
}