#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>
#include <time.h>
#include <chrono>
#include <algorithm>


#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/Player.h"
#include "lab_m1/Tema1/Map.h"
#include "lab_m1/Tema1/Enemy.h"
#include "lab_m1/Tema1/Healthbar.h"
# define M_PI           3.14159265358979323846  /* pi */

using namespace std;
using namespace m1;


Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 10;   // logic width
    logicSpace.height = 10;  // logic height

    healtbarX = logicSpace.x + logicSpace.width / 2 + 5;
    healtbarY = logicSpace.y + logicSpace.height / 2 + 3;

    enemyX = logicSpace.x + logicSpace.width / 2 - 5;
    enemyY = logicSpace.y + logicSpace.height / 2 - 5;

    told = std::chrono::system_clock::now();
    toldEnemy = std::chrono::system_clock::now();

    Mesh* map = Map::CreateMap("map", mapX, mapY);
    AddMeshToList(map);

    Mesh* obsPatrat = Map::CreateObsDrept("patrat", 1, 1);
    AddMeshToList(obsPatrat);

    Mesh* obsDreptU = Map::CreateObsDrept("dreptunghi", 1, 3);
    AddMeshToList(obsDreptU);

    CreatePlayer();

    CreateHealtbar(healtbarWidth, healtbarHeight);

    Mesh* bullet = Player::CreateBullet("bullet", bulletSize, bulletSize);
    AddMeshToList(bullet);

    Mesh* enemyBody = Enemy::CreateEnemyBody("enemyBody");
    AddMeshToList(enemyBody);

    Mesh* enemyArms = Enemy::CreateEnemyArms("enemyArms");
    AddMeshToList(enemyArms);

    AddObstacole();
}

void Tema1::CreatePlayer()
{
    /*Aceasta functie creaza mesh-urile pentru player si initializeaza pozitia lui in
    mijlocul viewport-ului*/

    player = PlayerS(logicSpace.x + logicSpace.width / 2,
        logicSpace.y + logicSpace.height / 2,
        0.7,
        0.7,
        0);
    Mesh* playerBody = Player::CreatePlayerBody("body");
    AddMeshToList(playerBody);
    player.name.push_back("body");

    Mesh* playerSenile = Player::CreatePlayerSenile("senile");
    AddMeshToList(playerSenile);
    player.name.push_back("senile");

    Mesh* playerWeapon = Player::CreatePlayerWeapon("weapon");
    AddMeshToList(playerWeapon);
    player.name.push_back("weapon");
}

void Tema1::CreateHealtbar(float x, float y)
{
    /*Aceasta functie creaza mesh-urile pentru healtbar */

    Mesh* healtbarFrame = Healthbar::CreateFrame("frame", x, y);
    AddMeshToList(healtbarFrame);

    Mesh* healtbarFill = Healthbar::CreateFill("fill", x / lifesNumber, y);
    AddMeshToList(healtbarFill);
}

void Tema1::DrawHealtbar(glm::mat3 visMatrix)
{
    /*Aceasta functie deseneaza rama healtbar-ului, si in functie de numarul de vieti deseneaza atatea
    forme de umplere in interiorul acestuia*/

    modelMatrix = visMatrix * transform2D::Translate(healtbarX, healtbarY);
    RenderMesh2D(meshes["frame"], shaders["VertexColor"], modelMatrix);
    for (int i = 0; i < life; i++) {
        modelMatrix = visMatrix * transform2D::Translate(healtbarX + 1.0f * (i * healtbarWidth / lifesNumber), healtbarY);
        RenderMesh2D(meshes["fill"], shaders["VertexColor"], modelMatrix);
    }
}

glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->Update();
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Tema1::CheckCollisionPlayerAll(PlayerS player, std::vector <ObsCoordonate> obstacole) {
    for (int i = 0; i < obstacole.size(); i++) {
        if (CheckCollisionPlayer(player, obstacole[i])) {
            return true;
        }
    }
    return false;
}

bool Tema1::CheckCollisionPlayer(PlayerS player, ObsCoordonate obstacol)
{
    /*Coliziunea implementata este una de tipul cerc(player) - dreptunghi(obstacol)*/

    float x = std::max(obstacol.minX, std::min(player.X, obstacol.maxX));
    float y = std::max(obstacol.minY, std::min(player.Y, obstacol.maxY));
    float distance = sqrt((x - player.X) * (x - player.X) + (y - player.Y) * (y - player.Y));
    return distance < player.scaleX;
}

bool Tema1::CheckCollisionEnemyMap(Enemies enemy, ObsCoordonate obstacol)
{
    /*Coliziunea implementata este una de tipul cerc(enemy) - dreptunghi(obstacol)*/

    float enemyCenterX = enemy.X + enemyScaleX;
    float enemyCenterY = enemy.Y + enemyScaleY;

    float x = std::max(obstacol.minX, std::min(enemyCenterX, obstacol.maxX));
    float y = std::max(obstacol.minY, std::min(enemyCenterY, obstacol.maxY));
    float distance = sqrt((x - enemyCenterX) * (x - enemyCenterX) + (y - enemyCenterY) * (y - enemyCenterY));
    return distance < enemyScaleX;
}

void Tema1::CheckCollisionEnemyMapAll(std::vector<Enemies> &enemies, std::vector<ObsCoordonate> obstacole)
{
    /*In cazul in care un jucator iese din harta, acesta moare si este sters din vector*/

    for (int i = 0; i < enemies.size(); i++) {
        for (int j = 0; j < obstacole.size(); j++) {
            if (CheckCollisionEnemyMap(enemies[i], obstacole[j])) {
                enemies.erase(enemies.begin() + i);
                i--;
                break;
            }
        }
    }
}

void Tema1::CheckCollisionBulletEnemyAll(std::vector<Bullet> &bullets, std::vector<Enemies> &enemies) {

    /*Daca exista coliziune intre un proiectil si un inamic, se sterg amandoua din vectori, 
    se mareste scorul si se afiseaza in consola*/
    bool deleteBullet = false;
    for (int i = 0; i < bullets.size(); i++) {
        for (int j = 0; j < enemies.size(); j++) {
            if (CheckCollisionBulletEnemy(bullets[i], enemies[j])) {
                score += 5;
                cout << "Score = " << score << '\n';
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

bool Tema1::CheckCollisionBulletEnemy(Bullet bullet, Enemies enemy)
{
    /*Coliziunea implementata este una de tipul cerc(proiectil) - cerc(inamic)*/

    float bulletCenterX = bullet.X + (bulletSize * bulletScaleX) / 2;
    float bulletCenterY = bullet.Y + (bulletSize * bulletScaleY) / 2;

    float distance = sqrt((bulletCenterX - enemy.X) * (bulletCenterX - enemy.X) +
                          (bulletCenterY - enemy.Y) * (bulletCenterY - enemy.Y));
    return distance < ((bulletSize * bulletScaleX) / 2 + enemyScaleX);
}

bool Tema1::CheckCollisionBulletObs(Bullet bullet, ObsCoordonate obstacol) {

    /*Coliziunea implementata este una de tipul cerc(proiectil) - dreptunchi(obstacol)*/

    float bulletCenterX = bullet.X + (bulletSize * bulletScaleX) / 2;
    float bulletCenterY = bullet.Y + (bulletSize * bulletScaleY) / 2;

    float x = std::max(obstacol.minX, std::min(bulletCenterX, obstacol.maxX));
    float y = std::max(obstacol.minY, std::min(bulletCenterY, obstacol.maxY));
    float distance = sqrt((x - bulletCenterX) * (x - bulletCenterX) + (y - bulletCenterY) * (y - bulletCenterY));
    return distance < (bulletSize* bulletScaleX) / 2;
}

void Tema1::CheckCollisionBulletObsAll(std::vector<Bullet> &bullets, std::vector<ObsCoordonate> &obstacole)
{
    /*Daca exista coliziune cu vreun obstacol, se sterge proiectilul din vector*/
    for (int i = 0; i < bullets.size(); i++) {
        for (int j = 0; j < obstacole.size(); j++) {
            if (CheckCollisionBulletObs(bullets[i], obstacole[j])) {
                bullets.erase(bullets.begin() + i);
                i--;
                break;
            }
        }
    }
}

bool Tema1::CheckCollisionPlayerEnemy(PlayerS player, Enemies enemy) {

    /*Coliziunea implementata este una de tipul cerc(player) - cerc(enemy)*/

    float distance = sqrt((player.X - enemy.X) * (player.X - enemy.X) +
        (player.Y - enemy.Y) * (player.Y - enemy.Y));
    return distance < (player.scaleX + enemyScaleX);
}

void Tema1::CheckCollisionPlayerEnemyAll(PlayerS player, std::vector<Enemies> &enemies) {
    /*In cazul in care este detectata coliziunea, se scade viata si este eliminat inamicul
      Daca viata scade la 0, jocul se opreste*/
    for (int i = 0; i < enemies.size(); i++) {
        if(CheckCollisionPlayerEnemy(player, enemies[i])){
            enemies.erase(enemies.begin() + i);
            i--;
            life--;
            if (life == 0) {
                std::cout << "Game Over! Your Score is " << score;
                exit(0);
            }

        }
    }
}

void Tema1::Update(float deltaTimeSeconds)
{
    // Compute the 2D visualization matrix
    glm::ivec2 resolution = window->GetResolution();
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    tnewEnemy = std::chrono::system_clock::now();
    if ((tnewEnemy - toldEnemy) / 1s > 2.5) {
        AddEnemies();
        toldEnemy = tnewEnemy;
    }

    CheckCollisionEnemyMapAll(enemies, mapCoord);
    CheckCollisionBulletObsAll(bullets, obsCoord);
    CheckCollisionBulletObsAll(bullets, mapCoord);
    CheckCollisionBulletEnemyAll(bullets, enemies);
    CheckCollisionPlayerEnemyAll(player, enemies);
    DrawScene(visMatrix);
}

void Tema1::FrameEnd()
{
}

void Tema1::AddEnemies() {
    /*Aceasta functie genereaza random pozitiile si viteza noilor inamici, iar in 
     cazul in care este la o distanta mai mica de 2 de jucator, il muta cu +/- 3 
     fata de acesta*/

    float randX = std::rand() % maxGenX - minGenX;
    if (player.X - randX < 2) {
        if (randX + 3 > maxGenX) {
            randX -= 3;
        }
        else {
            randX += 3;
        }
    }
    float randY = std::rand() % maxGenY - minGenY;
    if (player.Y - randY < 2) {
        if (randY + 3 > maxGenY) {
            randY -= 3;
        }
        else {
            randY += 3;
        }
    }

    float speed = (std::rand() % 30 + 10) ;
    enemies.push_back(Enemies(randX, randY, speed / 1000));
}

void Tema1::AddObstacole()
{
    //Se face o hardcodare a obstacolelor hartii
  
    //Adaugarea informatiilor obstacolelor intr-un vector cu elemente de tipul ObsCoordonate
    obsCoord.push_back(ObsCoordonate((logicSpace.width - mapX) / 2 + 25,
        (logicSpace.width - mapX) / 2 + 25 + 3,
        (logicSpace.height - mapY) / 2 + 3,
        (logicSpace.height - mapY) / 2 + 3 + 3,
        3, 1, 0));

    obsCoord.push_back(ObsCoordonate((logicSpace.width - mapX) / 2 + 4,
        (logicSpace.width - mapX) / 2 + 4 + 1,
        (logicSpace.height - mapY) / 2 + 14,
        (logicSpace.height - mapY) / 2 + 14 + 6,
        1, 2, 0));

    obsCoord.push_back(ObsCoordonate((logicSpace.width - mapX) / 2 + 6,
        (logicSpace.width - mapX) / 2 + 6 + 1,
        (logicSpace.height - mapY) / 2 + 3,
        (logicSpace.height - mapY) / 2 + 3 + 3,
        1, 1, 0));

    obsCoord.push_back(ObsCoordonate((logicSpace.width - mapX) / 2 + 3,
        (logicSpace.width - mapX) / 2 + 3 + 3,
        (logicSpace.height - mapY) / 2 + 5,
        (logicSpace.height - mapY) / 2 + 5 + 1,
        3, 0.33, 0));

    obsCoord.push_back(ObsCoordonate((logicSpace.width - mapX) / 2 + 13,
        (logicSpace.width - mapX) / 2 + 13 + 6,
        (logicSpace.height - mapY) / 2 + 8,
        (logicSpace.height - mapY) / 2 + 8 + 1,
        6, 0.33, 0));

    obsCoord.push_back(ObsCoordonate((logicSpace.width - mapX) / 2 + 16,
        (logicSpace.width - mapX) / 2 + 16 + 3,
        (logicSpace.height - mapY) / 2 + 0,
        (logicSpace.height - mapY) / 2 + 0 + 2,
        3, 0.66, 0));

    obsCoord.push_back(ObsCoordonate((logicSpace.width - mapX) / 2 + 19,
        (logicSpace.width - mapX) / 2 + 19 + 6,
        (logicSpace.height - mapY) / 2 + 16,
        (logicSpace.height - mapY) / 2 + 16 + 1,
        6, 0.33, 0));

    obsCoord.push_back(ObsCoordonate((logicSpace.width - mapX) / 2 + 20,
        (logicSpace.width - mapX) / 2 + 20 + 1,
        (logicSpace.height - mapY) / 2 + 16,
        (logicSpace.height - mapY) / 2 + 16 + 3,
        1, 1, 0));

    obsCoord.push_back(ObsCoordonate((logicSpace.width - mapX) / 2 + 24,
        (logicSpace.width - mapX) / 2 + 24 + 1,
        (logicSpace.height - mapY) / 2 + 16,
        (logicSpace.height - mapY) / 2 + 16 + 3,
        1, 1, 0));

    obsCoord.push_back(ObsCoordonate((logicSpace.width - mapX) / 2 + 13,
        (logicSpace.width - mapX) / 2 + 13 + 1,
        (logicSpace.height - mapY) / 2 + 19,
        (logicSpace.height - mapY) / 2 + 19 + 3,
        1, 1, 0));

    //Adaugarea informatiilor marginilor hartii intr-un vector cu elemente de tipul ObsCoordonate
    mapCoord.push_back(ObsCoordonate((logicSpace.width - mapX) / 2 - marginWidth,
        (logicSpace.width - mapX) / 2,
        (logicSpace.height - mapY) / 2 - marginWidth,
        (logicSpace.height - mapY) / 2 + mapY + marginWidth,
        0, 0, 0));

    mapCoord.push_back(ObsCoordonate((logicSpace.width - mapX) / 2 + mapX,
        (logicSpace.width - mapX) / 2 + mapX + marginWidth,
        (logicSpace.height - mapY) / 2 - marginWidth,
        (logicSpace.height - mapY) / 2 + mapY + marginWidth,
        0, 0, 0));

    mapCoord.push_back(ObsCoordonate((logicSpace.width - mapX) / 2,
        (logicSpace.width - mapX) / 2 + mapX,
        (logicSpace.height - mapY) / 2 - marginWidth,
        (logicSpace.height - mapY) / 2,
        0, 0, 0));

    mapCoord.push_back(ObsCoordonate((logicSpace.width - mapX) / 2,
        (logicSpace.width - mapX) / 2 + mapX,
        (logicSpace.height - mapY) / 2 + mapY,
        (logicSpace.height - mapY) / 2 + mapY + marginWidth,
        0, 0, 0));
}

void Tema1::DrawEnemies(glm::mat3 visMatrix)
{
    /*Se calculeaza de fiecare data unghiul de rotatie pentru ca inamicul sa fie orientat mereu
    spre jucator*/
    for (int i = 0; i < enemies.size(); i++) {
        enemyAngle = atan2(player.Y - enemies[i].Y, player.X - enemies[i].X);
        if (player.X - enemies[i].X != 0) {
            enemies[i].X += (player.X - enemies[i].X) / abs(player.X - enemies[i].X) * enemies[i].speed;
        }
        if (player.Y - enemies[i].Y != 0) {
            enemies[i].Y += (player.Y - enemies[i].Y) / abs(player.Y - enemies[i].Y) * enemies[i].speed;
        }
        modelMatrix = visMatrix * transform2D::Translate(enemies[i].X, enemies[i].Y) *
            transform2D::Scale(enemyScaleX, enemyScaleY) *
            transform2D::Rotate(enemyAngle - M_PI / 2);
        RenderMesh2D(meshes[enemies[i].body], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes[enemies[i].arms], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::DrawBullets(glm::mat3 visMatrix)
{
    /*Daca un proiectil a parcurs o anumita distanta, acesta este sters*/
    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].X += bullets[i].dist * cos(bullets[i].angle + M_PI / 2);
        bullets[i].Y += bullets[i].dist * sin(bullets[i].angle + M_PI / 2);
        modelMatrix = visMatrix * transform2D::Translate(bullets[i].X, bullets[i].Y) *
            transform2D::Scale(bulletScaleX, bulletScaleY) *
            transform2D::Rotate(bullets[i].angle);
        bullets[i].dist += 0.008;
        if (bullets[i].dist > bulletMaxDist) {
            bullets.erase(bullets.begin() + i);
            i--;
        }
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::DrawMap(glm::mat3 visMatrix)
{
    for (int i = 0; i < obsCoord.size(); i++) {
        modelMatrix = visMatrix * transform2D::Translate(obsCoord[i].minX, obsCoord[i].minY) *
            transform2D::Scale(obsCoord[i].scaleX, obsCoord[i].scaleY) *
            transform2D::Rotate(obsCoord[i].angle);
        RenderMesh2D(meshes["dreptunghi"], shaders["VertexColor"], modelMatrix);
    }

    modelMatrix = visMatrix * transform2D::Translate((logicSpace.width - mapX) / 2, (logicSpace.height - mapY) / 2);
    RenderMesh2D(meshes["map"], shaders["VertexColor"], modelMatrix);
}

void Tema1::DrawPlayer(glm::mat3 visMatrix)
{
    for (int i = 0; i < player.name.size(); i++) {
        modelMatrix = visMatrix * transform2D::Translate(player.X, player.Y) *
            transform2D::Scale(player.scaleX, player.scaleY) *
            transform2D::Rotate(player.angle);
        RenderMesh2D(meshes[player.name[i]], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::DrawScene(glm::mat3 visMatrix)
{
    DrawPlayer(visMatrix);
    DrawHealtbar(visMatrix);
    DrawBullets(visMatrix);
    DrawEnemies(visMatrix);
    DrawMap(visMatrix);
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    //Coordonatele viitoare ale jucatorului
    futurePlayer.X = player.X;
    futurePlayer.Y = player.Y;
    futurePlayer.scaleX = player.scaleX;
    futurePlayer.scaleY = player.scaleY;

    //Mutarea tank-ului la apasarea tastelor W, A, S, D
    //Pentru coliziuni se verifica pozitia urmatoare a jucatorului 
    if (window->KeyHold(GLFW_KEY_W)){
        futurePlayer.Y += deltaTime * speed;
        if (!CheckCollisionPlayerAll(futurePlayer, mapCoord) && 
            !CheckCollisionPlayerAll(futurePlayer, obsCoord)) {
            player.Y += deltaTime * speed;
            logicSpace.y += deltaTime * speed;
            healtbarY += deltaTime * speed;
        }
    }

    if (window->KeyHold(GLFW_KEY_S)){
        futurePlayer.Y -= deltaTime * speed;
        if (!CheckCollisionPlayerAll(futurePlayer, mapCoord) && 
            !CheckCollisionPlayerAll(futurePlayer, obsCoord)) {
            player.Y -= deltaTime * speed;
            logicSpace.y -= deltaTime * speed;
            healtbarY -= deltaTime * speed;
        }
    }

    if (window->KeyHold(GLFW_KEY_D)){
        futurePlayer.X += deltaTime * speed;
        if (!CheckCollisionPlayerAll(futurePlayer, mapCoord) && 
            !CheckCollisionPlayerAll(futurePlayer, obsCoord)) {
            player.X += deltaTime * speed;
            logicSpace.x += deltaTime * speed;
            healtbarX += deltaTime * speed;
        }
    }

    if (window->KeyHold(GLFW_KEY_A)){
        futurePlayer.X -= deltaTime * speed;
        if (!CheckCollisionPlayerAll(futurePlayer, mapCoord) && 
            !CheckCollisionPlayerAll(futurePlayer, obsCoord)) {
            player.X -= deltaTime * speed;
            logicSpace.x -= deltaTime * speed;
            healtbarX -= deltaTime * speed;
        }
    }
}

void Tema1::OnKeyPress(int key, int mods)
{
}

void Tema1::OnKeyRelease(int key, int mods)
{
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    /*Calcularea unghiului dupa care trebuie rotit jucatorul in
    functie de pozitia mouse-ului*/
    glm::ivec2 resolution = window->GetResolution();
    player.angle = atan2(resolution.x / 2 - (mouseX + deltaX), 
                         resolution.y / 2 - (mouseY + deltaY));

}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    tnew = std::chrono::system_clock::now();
    if ((tnew - told) / 1ms > 500 && button == GLFW_MOUSE_BUTTON_2) {
        bullets.push_back(Bullet(player.X + 1.5 * player.scaleY * cos(player.angle + M_PI / 2) - 0.15 * player.scaleX * sin(player.angle + M_PI / 2),
            player.Y + 1.5 * player.scaleY * sin(player.angle + M_PI / 2) + 0.15 * player.scaleX * cos(player.angle + M_PI / 2),
            player.angle,
            0));
        told = tnew;
    }


}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}
