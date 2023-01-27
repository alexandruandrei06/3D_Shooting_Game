#include "Hud.h"

#include "core/engine.h"
#include "utils/gl_utils.h"

//Define cu vectorii de culori pentru healtbar si timebar
#define TimeColor glm::vec3(1, 0.549, 0.121)
#define LifeColor glm::vec3(0.121, 0.690, 1)


Mesh* Hud::CreateFrameTime(const std::string& name, float x, float y){
    std::vector<VertexFormat> vertices =
    {

        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(TimeColor)), //0
        VertexFormat(glm::vec3(0, y, 0), glm::vec3(TimeColor)), //1
        VertexFormat(glm::vec3(x, y, 0), glm::vec3(TimeColor)), //2
        VertexFormat(glm::vec3(x, 0, 0), glm::vec3(TimeColor)), //3

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

Mesh* Hud::CreateFrameLife(
    const std::string& name, float x, float y)
{
    std::vector<VertexFormat> vertices =
    {

        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(LifeColor)), //0
        VertexFormat(glm::vec3(0, y, 0), glm::vec3(LifeColor)), //1
        VertexFormat(glm::vec3(x, y, 0), glm::vec3(LifeColor)), //2
        VertexFormat(glm::vec3(x, 0, 0), glm::vec3(LifeColor)), //3

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

Mesh* Hud::CreateFillLife(
    const std::string& name, float x, float y)
{
    std::vector<VertexFormat> vertices =
    {

        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(LifeColor)), //0
        VertexFormat(glm::vec3(0, y, 0), glm::vec3(LifeColor)), //1
        VertexFormat(glm::vec3(x, y, 0), glm::vec3(LifeColor)), //2
        VertexFormat(glm::vec3(x, 0, 0), glm::vec3(LifeColor)), //3

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

Mesh* Hud::CreateFillTime(const std::string& name, float x, float y)
{
    std::vector<VertexFormat> vertices =
    {

        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(TimeColor)), //0
        VertexFormat(glm::vec3(0, y, 0), glm::vec3(TimeColor)), //1
        VertexFormat(glm::vec3(x, y, 0), glm::vec3(TimeColor)), //2
        VertexFormat(glm::vec3(x, 0, 0), glm::vec3(TimeColor)), //3

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