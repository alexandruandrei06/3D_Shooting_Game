#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <chrono>
#include "lab_m1/Tema2/T2_Map.h"
#include "lab_m1/Tema2/T2_Player.h"
#include "lab_m1/Tema2/T2_transform2D.h"
#include "lab_m1/Tema2/Camera.h"
#include "lab_m1/Tema2/Hud.h"

#define enemyColor glm::vec3(0.686, 0.992, 0.992)
#define HEAD 1
#define BODY_ARM 2
#define ARM 3
#define LEGS 4

using namespace std;
using namespace m1;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
    toldFire = std::chrono::system_clock::now();

    Mesh* sphere = new Mesh("sphere");
    sphere->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
    AddMeshToList(sphere);

    Mesh* map = T2_Map::CreateMapCell("map_cell");
    AddMeshToList(map);

    Mesh* wall = T2_Map::CreateMapWall("map_wall");
    AddMeshToList(wall);

    ReadMaze();
    CreatePlayer();
    CreateHud(hudWidth, hudHeight);
    AddObstacoleMap();
    AddEnemies();

    {
        Shader* shader = new Shader("Tema2_Shader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "T2_shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "T2_shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }


    //Inaltimea la care e satata camera este inaltimea jucatorului
    float inaltimeCamera = (2 + 2 + 0.9) * playerScale;
    camera = new implementedCamera::Camera();
    camera->Set(glm::vec3(0, inaltimeCamera, 1), glm::vec3(0, inaltimeCamera, 0), glm::vec3(0, 1, 0));

    //Pozitionarea initiala a jucatorului dupa camera
    glm::vec3 playerPos = camera->position;
    player_x = playerPos.x;
    player_z = playerPos.z;

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 100.f);

    //HUD-----------------------------
    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 4;   // logic width
    logicSpace.height = 4;  // logic height

}

void Tema2::ReadMaze() {
    /*Se citeste din memorie o harta dintre cele 10, in mod aleatoriu
      harta are urmatoarea legenda:
      0 = cale libera
      1 = perete
      2 = inamic care se misca pe directia N-S
      3 = inamic care se misca pe directia V-E*/
    int mazeId = std::rand() % 10;
    char name[50];
    sprintf(name, "./src/lab_m1/Tema2/Mazes/maze%d.txt", mazeId);
    std::cout << name;
    ifstream file;
    file.open(name);
    file >> Map_Grid_Y >> Map_Grid_X;
    for (int i = 0; i < Map_Grid_X; i++) {
        for (int j = 0; j < Map_Grid_Y; j++) {
            file >> maze[i][j];
        }
    }
    file.close();
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::FrameEnd()
{

}

void Tema2::CreatePlayer() {
    Mesh* head = T2_Player::CreatePlayerPart("player_head", HEAD);
    AddMeshToList(head);

    Mesh* leg = T2_Player::CreatePlayerPart("player_leg", LEGS);
    AddMeshToList(leg);

    Mesh* arm = T2_Player::CreatePlayerPart("player_arm", ARM);
    AddMeshToList(arm);

    Mesh* body = T2_Player::CreatePlayerPart("player_body", BODY_ARM);
    AddMeshToList(body);
}

void Tema2::CreateHud(float x, float y) {
    Mesh* healtbarFrame = Hud::CreateFrameLife("lifeFrame", x, y);
    AddMeshToList(healtbarFrame);

    Mesh* healtbarFill = Hud::CreateFillLife("lifeFill", x / lifesNumber, y);
    AddMeshToList(healtbarFill);

    Mesh* timeFrame = Hud::CreateFrameTime("timeFrame", x, y);
    AddMeshToList(timeFrame);

    Mesh* timeFill = Hud::CreateFillTime("timeFill", x / timeNumber, y);
    AddMeshToList(timeFill);
}

void Tema2::AddEnemies()
{
    for (int i = 0; i < Map_Grid_X; ++i) {
        for (int j = 0; j < Map_Grid_Y; ++j) {
            if (maze[i][j] == 2) {
                enemies.push_back(Enemies(i + 0.5, 0, j + 0.5, enemyScale, 0, 1));
            }
            else if (maze[i][j] == 3) {
                enemies.push_back(Enemies(i + 0.5, 0, j + 0.5, enemyScale, 1, 0));
            }
        }
    }
}

void Tema2::AddObstacoleMap()
{
    for (int i = 0; i < Map_Grid_X; ++i) {
        for (int j = 0; j < Map_Grid_Y; ++j) {
            if (maze[i][j] == 1) {
                obsCoord.push_back(ObsCoordonate(i, 0, j));
            }
        }
    }
}

void Tema2::DrawPlayer(glm::mat4 visMatrix)
{
    //Corp
    modelMatrix = visMatrix;
    modelMatrix = glm::translate(modelMatrix, glm::vec3(player_x, player_y, player_z));
    modelMatrix = glm::rotate(modelMatrix, playerAngle, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.25 * playerScale, 2 * playerScale, 1.2 * playerScale));
    RenderMesh(meshes["player_body"], shaders["VertexColor"], modelMatrix);

    //Cap
    modelMatrix = visMatrix;
    modelMatrix = glm::translate(modelMatrix, glm::vec3(player_x, player_y + (2.0 + 0.9) * playerScale / 2, player_z));
    modelMatrix = glm::rotate(modelMatrix, playerAngle, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.25 * playerScale, 0.9 * playerScale, 1.2 * playerScale));
    RenderMesh(meshes["player_head"], shaders["VertexColor"], modelMatrix);

    //Picioare
    modelMatrix = visMatrix;
    modelMatrix = glm::translate(modelMatrix, glm::vec3(player_x, player_y - (2 + 2) * playerScale / 2, player_z));
    modelMatrix = glm::rotate(modelMatrix, playerAngle, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.25 * playerScale, 2 * playerScale, 1.2 * playerScale));
    RenderMesh(meshes["player_leg"], shaders["VertexColor"], modelMatrix);

    //Mana stanga
    modelMatrix = visMatrix;
    modelMatrix = glm::translate(modelMatrix, glm::vec3(player_x, player_y, player_z));
    modelMatrix = glm::rotate(modelMatrix, playerAngle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-(1 + 1.25) * playerScale / 2, -(2 - 0.8) * playerScale / 2, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1 * playerScale, 0.8 * playerScale, 1.2 * playerScale));
    RenderMesh(meshes["player_head"], shaders["VertexColor"], modelMatrix);

    //Bratul stang
    modelMatrix = visMatrix;
    modelMatrix = glm::translate(modelMatrix, glm::vec3(player_x, player_y, player_z));
    modelMatrix = glm::rotate(modelMatrix, playerAngle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-(1 + 1.25) * playerScale / 2, -(2 - 0.8 - 2) * playerScale / 2, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1 * playerScale, (2 - 0.8) * playerScale, 1.2 * playerScale));
    RenderMesh(meshes["player_body"], shaders["VertexColor"], modelMatrix);

    //Mana dreapta
    modelMatrix = visMatrix;
    modelMatrix = glm::translate(modelMatrix, glm::vec3(player_x, player_y, player_z));
    modelMatrix = glm::rotate(modelMatrix, playerAngle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3((1 + 1.25) * playerScale / 2, -(2 - 0.8) * playerScale / 2, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1 * playerScale, 0.8 * playerScale, 1.2 * playerScale));
    RenderMesh(meshes["player_head"], shaders["VertexColor"], modelMatrix);

    //Bratul drept
    modelMatrix = visMatrix;
    modelMatrix = glm::translate(modelMatrix, glm::vec3(player_x, player_y, player_z));
    modelMatrix = glm::rotate(modelMatrix, playerAngle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3((1 + 1.25) * playerScale / 2, -(2 - 0.8 - 2) * playerScale / 2, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1 * playerScale, (2 - 0.8) * playerScale, 1.2 * playerScale));
    RenderMesh(meshes["player_body"], shaders["VertexColor"], modelMatrix);

}

void Tema2::DrawMap(glm::mat4 visMatrix)
{
    for (int i = 0; i < Map_Grid_X; i++) {
        for (int j = 0; j < Map_Grid_Y; j++) {
            modelMatrix = visMatrix;
            modelMatrix = glm::translate(modelMatrix, glm::vec3(i, 0, j));
            RenderMesh(meshes["map_cell"], shaders["VertexColor"], modelMatrix);
            if (maze[i][j] == 1) {
                modelMatrix = visMatrix;
                modelMatrix = glm::translate(modelMatrix, glm::vec3(i, 0, j));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 2, 1));
                RenderMesh(meshes["map_wall"], shaders["VertexColor"], modelMatrix);
            }
        }
    }
}

void Tema2::DrawBullets(glm::mat4 visMatrix) {
    /*Daca un proiectil a parcurs o anumita distanta, acesta este sters*/
    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].X += bullets[i].dist * sin(bullets[i].angle + M_PI);
        bullets[i].Z += bullets[i].dist * cos(bullets[i].angle + M_PI);
        bullets[i].Y += bullets[i].dist * sin(bullets[i].angleOY);
        bullets[i].dist += 0.008;
        if (bullets[i].dist > bulletMaxDist || CheckCollisionBulletObsAll(bullets[i])) {
            bullets.erase(bullets.begin() + i);
            i--;
        }
        else {
            modelMatrix = visMatrix;
            modelMatrix = glm::translate(modelMatrix, glm::vec3(bullets[i].X, bullets[i].Y, bullets[i].Z));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(tintaScale));
            RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
        }
    }
}

void Tema2::DrawTinta(glm::mat4 visMatrix) {
    modelMatrix = visMatrix;
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::scale(modelMatrix, glm::vec3(tintaScale));
    RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
}

void Tema2::DrawEnemies(glm::mat4 visMatrix, float deltaTimeSeconds) {
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].x += enemies[i].direction_x * deltaTimeSeconds * enemies[i].speed * enemies[i].direction;
        enemies[i].z += enemies[i].direction_z * deltaTimeSeconds * enemies[i].speed * enemies[i].direction;
        if (CheckCollisionEnemyMapAll(enemies[i])) {
            enemies[i].x -= enemies[i].direction_x * deltaTimeSeconds * enemies[i].speed * enemies[i].direction;
            enemies[i].z -= enemies[i].direction_z * deltaTimeSeconds * enemies[i].speed * enemies[i].direction;
            enemies[i].direction *= -1;
        }

        glm::mat4 modelMatrix = visMatrix;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(enemies[i].x, enemies[i].y, enemies[i].z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(enemies[i].scale_1));
        RenderMesh(meshes["sphere"], shaders["Color"], modelMatrix, enemyColor);

        modelMatrix = visMatrix;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(enemies[i].x, enemies[i].y + (enemies[i].scale_1 + enemies[i].scale_2) / 2 - 0.1, enemies[i].z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(enemies[i].scale_2));
        RenderMesh(meshes["sphere"], shaders["Color"], modelMatrix, enemyColor);

        modelMatrix = visMatrix;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(enemies[i].x,
            enemies[i].y + (enemies[i].scale_1 + enemies[i].scale_2 * 2 + enemies[i].scale_3) / 2 - 0.15,
            enemies[i].z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(enemies[i].scale_3));
        RenderMesh(meshes["sphere"], shaders["Color"], modelMatrix, enemyColor);
    }
}

void Tema2::DrawEnemiesDead() {
    glm::mat4 visMatrix = glm::mat4(1);
    for (int i = 0; i < DeadEnemies.size(); i++) {
        glm::mat4 modelMatrix = visMatrix;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(DeadEnemies[i].x, DeadEnemies[i].y, DeadEnemies[i].z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(DeadEnemies[i].scale_1));
        RenderMeshExplosion(meshes["sphere"], shaders["Tema2_Shader"], modelMatrix, enemyColor, DeadEnemies[i].deadTime);

        modelMatrix = visMatrix;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(DeadEnemies[i].x, DeadEnemies[i].y + (DeadEnemies[i].scale_1 + DeadEnemies[i].scale_2) / 2 - 0.1, DeadEnemies[i].z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(DeadEnemies[i].scale_2));
        RenderMeshExplosion(meshes["sphere"], shaders["Tema2_Shader"], modelMatrix, enemyColor, DeadEnemies[i].deadTime);

        modelMatrix = visMatrix;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(DeadEnemies[i].x,
            DeadEnemies[i].y + (DeadEnemies[i].scale_1 + DeadEnemies[i].scale_2 * 2 + DeadEnemies[i].scale_3) / 2 - 0.15,
            DeadEnemies[i].z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(DeadEnemies[i].scale_3));
        RenderMeshExplosion(meshes["sphere"], shaders["Tema2_Shader"], modelMatrix, enemyColor, DeadEnemies[i].deadTime);

        DeadEnemies[i].deadTime += 0.1;
        if (DeadEnemies[i].deadTime > 7) {
            DeadEnemies.erase(DeadEnemies.begin() + i);
            i--;
        }
    }
}

void Tema2::DrawHud() {
    auto camera2D = GetSceneCamera();
    camera2D->SetPosition(glm::vec3(0, 0, 50));
    camera2D->SetRotation(glm::vec3(0, 0, 0));
    camera2D->Update();
    GetCameraInput()->SetActive(false);

    glm::ivec2 resolution = window->GetResolution();
    viewSpace = ViewportSpace(3 * resolution.x / 4, 3 * resolution.y / 4, resolution.x / 4, resolution.y / 4);
    SetViewportArea(viewSpace, glm::vec3(0.5f), true);

    glm::mat3 visMatrixHud = glm::mat3(1);
    visMatrixHud *= VisualizationViewPort(logicSpace, viewSpace);
    DrawHudScene(visMatrixHud);
    GetCameraInput()->SetActive(true);
}

void Tema2::DrawHudScene(glm::mat3 visMatrix) {

    glm::mat3 modelMatrix = visMatrix * T2_transform2D::Translate(healthbarX, healthbarY);
    RenderMesh2D(meshes["lifeFrame"], shaders["VertexColor"], modelMatrix);

    for (int i = 0; i < life; i++) {
        modelMatrix = visMatrix * T2_transform2D::Translate(healthbarX + 1.0f * (i * hudWidth / lifesNumber), healthbarY);
        RenderMesh2D(meshes["lifeFill"], shaders["VertexColor"], modelMatrix);
    }

    modelMatrix = visMatrix * T2_transform2D::Translate(timeX, timeY);
    RenderMesh2D(meshes["timeFrame"], shaders["VertexColor"], modelMatrix);

    for (int i = 0; i < timeHud / 5; i++) {
        modelMatrix = visMatrix * T2_transform2D::Translate(timeX + 1.0f * (i * hudWidth / timeNumber), timeY);
        RenderMesh2D(meshes["timeFill"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema2::MovePlayer() {
    glm::vec3 playerPos;
    if (cameraSwitch) {
        playerPos = camera->GetTargetPosition();
        player_x = playerPos.x;
        player_z = playerPos.z;
    }
    else {
        playerPos = camera->position;
        player_x = playerPos.x;
        player_z = playerPos.z;
    }
}

void Tema2::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

glm::mat3 Tema2::VisualizationViewPort(const LogicSpace& logicSpace, const ViewportSpace& viewSpace) {
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}

bool Tema2::CheckCollisionPlayer(ObsCoordonate obstacol){

    /*Coliziunea implementata este de tipul sfera(player) - paralelipiped dreptunghic(perete)*/

    if (player_x < 0 || player_z < 0) {
        return true;
    }

    float sphere_radius = 1.25 * playerScale / 2 + 1.2 * playerScale;

    float x = std::max(obstacol.minX, std::min(player_x, obstacol.maxX));
    float z = std::max(obstacol.minZ, std::min(player_z, obstacol.maxZ));

    float distance = sqrt((x - player_x) * (x - player_x) + (z - player_z) * (z - player_z));
    return distance < sphere_radius;
}

bool Tema2::CheckCollisionPlayerAll() {
    for (int i = 0; i < obsCoord.size(); ++i) {
        if (CheckCollisionPlayer(obsCoord[i])) {
            return true;
        }
    }
    return false;
}

bool Tema2::CheckCollisionPlayerEnemy(Enemies enemy) {
    /*Coliziunea implementata este de tipul sfera(player) - sfera(inamic)
      Se verifica fiecare sfera a omului de zapada cu sfera player-ului*/

    float sphere_radius = 1.25 * playerScale / 2 + 1.2 * playerScale;

    //Prima sfera
    float y1 = enemy.y + (enemy.scale_1 + enemy.scale_2 * 2 + enemy.scale_3) / 2 - 0.15;
    float distance1 = sqrt((player_x - enemy.x) * (player_x - enemy.x) +
        (player_y - y1) * (player_y - y1) +
        (player_z - enemy.z) * (player_z - enemy.z));

    //Sfera 2
    float y2 = enemy.y + (enemy.scale_1 + enemy.scale_2) / 2 - 0.1;
    float distance2 = sqrt((player_x - enemy.x) * (player_x - enemy.x) +
        (player_y - y2) * (player_y - y2) +
        (player_z - enemy.z) * (player_z - enemy.z));

    //Baza
    float distance3 = sqrt((player_x - enemy.x) * (player_x - enemy.x) +
        (player_y - enemy.y) * (player_y - enemy.y) +
        (player_z - enemy.z) * (player_z - enemy.z));

    return (distance1 < (sphere_radius + enemy.scale_3)) ||
        (distance2 < (sphere_radius + enemy.scale_2)) ||
        (distance3 < (sphere_radius + enemy.scale_1));
}

void Tema2::CheckCollisionPlayerEnemyAll() {
    for (int i = 0; i < enemies.size(); i++) {
        if (CheckCollisionPlayerEnemy(enemies[i])) {
            enemies.erase(enemies.begin() + i);
            i--;
            life--;
            if (life == 0) {
                std::cout << "Game over! You lose!";
                exit(1);
            }
        }
    }
}

bool Tema2::CheckCollisionEnemyMap(ObsCoordonate obstacol, Enemies enemy) {
    /*Coliziunea implementata este de tipul sfera(enemy) - paralelipiped dreptunghic(perete)
      Se face verificarea doar cu sfera de jos, fiind cea mai mare, ea ar intra 
      prima in perete*/

    float x = std::max(obstacol.minX, std::min(enemy.x, obstacol.maxX));
    float z = std::max(obstacol.minZ, std::min(enemy.z, obstacol.maxZ));

    float distance = sqrt((x - enemy.x) * (x - enemy.x) + (z - enemy.z) * (z - enemy.z));
    return distance < enemy.scale_1;
}

bool Tema2::CheckCollisionEnemyMapAll(Enemies enemy) {
    for (int i = 0; i < obsCoord.size(); ++i) {
        if (CheckCollisionEnemyMap(obsCoord[i], enemy)) {
            return true;
        }
    }

    if (enemy.x < 0.5 || enemy.z < 0.5 || enemy.x > Map_Grid_X - 0.5 || enemy.z > Map_Grid_Y - 0.5) {
        return true;
    }

    return false;
}

bool Tema2::CheckCollisionBulletObs(Bullet bullet, ObsCoordonate obstacol) {
    /*Coliziunea implementata este de tipul sfera(bullet) - paralelipiped dreptunghic(perete)*/

    float x = std::max(obstacol.minX, std::min(bullet.X, obstacol.maxX));
    float z = std::max(obstacol.minZ, std::min(bullet.Z, obstacol.maxZ));

    if (bullet.Y < 0) {
       return true;
    }

    float distance = sqrt((x - bullet.X) * (x - bullet.X) + (z - bullet.Z) * (z - bullet.Z));
    return distance < tintaScale;
}

bool Tema2::CheckCollisionBulletObsAll(Bullet bullet) {
    for (int i = 0; i < obsCoord.size(); ++i) {
        if (CheckCollisionBulletObs(bullet, obsCoord[i])) {
            return true;
        }
    }
    return false;
}

bool Tema2::CheckCollisionBulletEnemy(Bullet bullet, Enemies enemy) {
    /*Coliziunea implementata este de tipul sfera - sfera
      Verificarea se face cu toate cele 3 sfere ale omului de zapada*/
    //Prima sfera
    float y1 = enemy.y + (enemy.scale_1 + enemy.scale_2 * 2 + enemy.scale_3) / 2 - 0.15;
    float distance1 = sqrt((bullet.X - enemy.x) * (bullet.X - enemy.x) +
                           (bullet.Y - y1) * (bullet.Y - y1) +
                           (bullet.Z - enemy.z) * (bullet.Z - enemy.z));
    //Sfera 2
    float y2 = enemy.y + (enemy.scale_1 + enemy.scale_2) / 2 - 0.1;
    float distance2 = sqrt((bullet.X - enemy.x) * (bullet.X - enemy.x) +
        (bullet.Y - y2) * (bullet.Y - y2) +
        (bullet.Z - enemy.z) * (bullet.Z - enemy.z));

    //Baza
    float distance3 = sqrt((bullet.X - enemy.x) * (bullet.X - enemy.x) +
        (bullet.Y - enemy.y) * (bullet.Y - enemy.y) +
        (bullet.Z - enemy.z) * (bullet.Z - enemy.z));

    return (distance1 < (tintaScale + enemy.scale_3)) ||
           (distance2 < (tintaScale + enemy.scale_2)) ||
           (distance3 < (tintaScale + enemy.scale_1));
}

void Tema2::CheckCollisionBulletEnemyAll() {
    bool deleteBullet = false;
    for (int i = 0; i < bullets.size(); i++) {
        for (int j = 0; j < enemies.size(); j++) {
            if (CheckCollisionBulletEnemy(bullets[i], enemies[j])) {
                DeadEnemies.push_back(enemies[j]);
                enemies.erase(enemies.begin() + j);
                deleteBullet = true;
                break;
            }
            
        }
        if (deleteBullet) {
            bullets.erase(bullets.begin() + i);
            i--;
        }
    }
}

bool Tema2::CheckCollisionCamera() {
    
    /*Coliziunea implementata este una de tipul punct(camera) - Paralelipiped dreptunghic(perete)*/
    glm::vec3 cameraPos;
    cameraPos = camera->position;
    for (int i = 0; i < obsCoord.size(); ++i) {
        if ((cameraPos.x >= obsCoord[i].minX && cameraPos.x <= obsCoord[i].maxX) &&
            (cameraPos.y >= obsCoord[i].minY && cameraPos.y <= obsCoord[i].maxY) &&
            (cameraPos.z >= obsCoord[i].minZ && cameraPos.z <= obsCoord[i].maxZ) || cameraPos.y < 0) {
            return true;
        }
    }
    return false;
}

void Tema2::Update(float deltaTimeSeconds)
{   
    if (player_x > Map_Grid_X || player_z > Map_Grid_Y) {
        std::cout << "Congratulations! You won!";
        exit(1);
    }

    CheckCollisionBulletEnemyAll();
    CheckCollisionPlayerEnemyAll();
    visMatrix = glm::mat4(1);
    DrawMap(visMatrix);
    if (cameraSwitch) {
        DrawPlayer(visMatrix);
    }
    else {
        DrawTinta(visMatrix);
    }

    DrawBullets(visMatrix);
    DrawEnemies(visMatrix, deltaTimeSeconds);
    DrawEnemiesDead();

    DrawHud();
    
    timeHud--;
    if (timeHud == 0) {
        std::cout << "Game Over! Time's up!";
        exit(1);
    }
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3fv(shader->GetUniformLocation("color"), 1, glm::value_ptr(color));

    mesh->Render();
}

void Tema2::RenderMeshExplosion(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, float deltaTimeSeconds)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3fv(shader->GetUniformLocation("object_color"), 1, glm::value_ptr(color));

    GLfloat time = glGetUniformLocation(shader->program, "time");
    glUniform1f(time, deltaTimeSeconds);
    mesh->Render();
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    //Se misca camera, iar jucatorul se misca in functie de camera
    float cameraSpeed = 1.5f;
    glm::vec3 cameraPos;
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (window->KeyHold(GLFW_KEY_W)) {
            camera->MoveForward(cameraSpeed * deltaTime);
            MovePlayer();

            if (CheckCollisionPlayerAll() || CheckCollisionCamera()) {
                camera->MoveForward((-1) * cameraSpeed * deltaTime);
                MovePlayer();
            }
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            camera->TranslateRight((-1) * cameraSpeed * deltaTime);
            MovePlayer();

            if (CheckCollisionPlayerAll() || CheckCollisionCamera()) {
                camera->TranslateRight(cameraSpeed * deltaTime);
                MovePlayer();
            }
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            camera->MoveForward((-1) * cameraSpeed * deltaTime);
            MovePlayer();

            if (CheckCollisionPlayerAll() || CheckCollisionCamera()) {
                camera->MoveForward(cameraSpeed * deltaTime);
                MovePlayer();
            }
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            camera->TranslateRight(cameraSpeed * deltaTime);
            MovePlayer();

            if (CheckCollisionPlayerAll() || CheckCollisionCamera()) {
                camera->TranslateRight((-1) * cameraSpeed * deltaTime);
                MovePlayer();
            }
        }
    }
}

void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        cameraSwitch = !cameraSwitch;
        if (cameraSwitch) {
            camera->TranslateForward(-camera->distanceToTarget);
        }
        else {
            camera->TranslateForward(camera->distanceToTarget);
        }
        
    }
}

void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{   
    // Add mouse move event
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        float sensivityOX = 0.002f;
        float sensivityOY = 0.002f;
        if (cameraSwitch) {
            camera->RotateThirdPerson_OX(sensivityOX * -deltaY);
        }else {
            camera->RotateFirstPerson_OX(sensivityOX * -deltaY);
        }

        playerAngleOY += sensivityOX * -deltaY;
        if (CheckCollisionCamera()) {
            if (cameraSwitch) {
                camera->RotateThirdPerson_OX(sensivityOX * +deltaY);
            }
            else {
                camera->RotateFirstPerson_OX(sensivityOX * +deltaY);
            }
            playerAngleOY += sensivityOX * deltaY;
        }

        if (cameraSwitch) {
            camera->RotateThirdPerson_OY(sensivityOY * -deltaX);
        }else {
            camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
        }
       
        playerAngle += sensivityOY * -deltaX;
        if (CheckCollisionCamera()) {
            if (cameraSwitch) {
                camera->RotateThirdPerson_OY(sensivityOY * deltaX);
            }else {
                camera->RotateFirstPerson_OY(sensivityOY * deltaX);
            }
            playerAngle += sensivityOY * deltaX;
            
        }
    }
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    tnewFire = std::chrono::system_clock::now();
    if (!cameraSwitch) {
        if ((tnewFire - toldFire) / 1ms > 150 && button == GLFW_MOUSE_BUTTON_2) {
            glm::vec3 cameraPoz = camera->GetTargetPosition();
            bullets.push_back(Bullet(cameraPoz.x, cameraPoz.y, cameraPoz.z, playerAngle, playerAngleOY, 0));
            toldFire = tnewFire;
        }
    }
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
