#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema2/Camera.h"
#include <vector>
#include <time.h>
#include <chrono>


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        //Structura folosita pentru retinerea informatiilor despre un obstacol
        struct ObsCoordonate
        {
            ObsCoordonate() : minX(0), maxX(0), minY(0), maxY(0), minZ(0), maxZ(0) {}
            ObsCoordonate(float x, float y, float z)
                : minX(x), minY(y), minZ(z), maxX(x + 1), maxY(y + 2), maxZ(z + 1) {}
            float minX;
            float maxX;
            float minY;
            float maxY;
            float minZ;
            float maxZ;
        };

        struct PlayerCoordonate
        {
            PlayerCoordonate() : minX(0), maxX(0), minY(0), maxY(0), minZ(0), maxZ(0) {}
            PlayerCoordonate(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z)
                : minX(min_x), minY(min_y), minZ(min_z), maxX(max_x), maxY(max_y), maxZ(max_z) {}
            float minX;
            float maxX;
            float minY;
            float maxY;
            float minZ;
            float maxZ;
        };

        struct Enemies
        {
            Enemies(float x, float y, float z, float enemyScale, float direction_x, float direction_z)
                : x(x), y(y + enemyScale/2), z(z), scale_1(1* enemyScale), scale_2(0.7* enemyScale), scale_3(0.5* enemyScale),
                 direction_x(direction_x), direction_z(direction_z), direction(1), deadTime(1.f), speed(1.3f){}
            float x;
            float y;
            float z;
            float scale_1;
            float scale_2;
            float scale_3;
            float direction_x;
            float direction_z;
            float direction;
            float deadTime;
            float speed;
        };

        struct Bullet
        {
            Bullet() : X(0), Y(0), Z(0), angle(0), dist(0) {}
            Bullet(float x, float y, float z, float angle, float angleOY, float dist)
                : X(x), Y(y), Z(z), angle(angle), angleOY(angleOY), dist(dist) {}
            float X;
            float Y;
            float Z;
            float angle;
            float angleOY;
            float dist;
        };

        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

        //Fuctie care citeste harta 
        void ReadMaze();

        void FrameStart() override;
        void FrameEnd() override;

        //Functie pentru initializarea mesh-urilor folosite la player
        void CreatePlayer(); //Player

        //Functie pentru initializarea mesh-urilor folosite pentru HUD
        void CreateHud(float x, float y); //HUD

        //Functie pentru adaugarea unui inamic dupa citirea matricei hartii
        void AddEnemies();  //Enemy

        //Functie pentru adaugarea unui perete dupa citirea matricei hartii
        void AddObstacoleMap(); //Map

        //Functie pentru desenarea jucatorului
        void DrawPlayer(glm::mat4 visMatrix); //Player

        //Functie pentru desenarea hartii
        void DrawMap(glm::mat4 visMatrix); //Map

        //Functie pentru desenarea proiectilelor
        void DrawBullets(glm::mat4 visMatrix); //Bullet

        //Functie pentru desenarea tintei din first-person
        void DrawTinta(glm::mat4 visMatrix); //Player

        //Functie pentru desenarea inamicilor care sunt in viata
        void DrawEnemies(glm::mat4 visMatrix, float deltaTimeSeconds); //Enemy

        /*Functie pentru desenarea inamicilor care au murit, acestia nu se mai deplaseaza
        si se schimba shader-ul pentru explozie*/
        void DrawEnemiesDead(); //Enemy

        //Functii pentru desenarea HUD-ului
        void DrawHud(); //HUD
        void DrawHudScene(glm::mat3 visMatrix); //HUD

        //Functie pentru miscarea jucatorului in functie de pozitia camerei
        void MovePlayer(); //Player

        //Functii pentru setarea viewport-ului pentru HUD
        glm::mat3 VisualizationViewPort(const LogicSpace& logicSpace, const ViewportSpace& viewSpace); //HUD
        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear); //HUD

        //Coliziuni
        bool CheckCollisionPlayer(ObsCoordonate obstacol);
        bool CheckCollisionPlayerAll();

        bool CheckCollisionPlayerEnemy(Enemies enemy);
        void CheckCollisionPlayerEnemyAll();

        bool CheckCollisionEnemyMap(ObsCoordonate obstacol, Enemies enemy);
        bool CheckCollisionEnemyMapAll(Enemies enemy);

        bool CheckCollisionBulletObs(Bullet bullet, ObsCoordonate obstacol);
        bool CheckCollisionBulletObsAll(Bullet bullet);

        bool CheckCollisionBulletEnemy(Bullet bullet, Enemies enemy);
        void CheckCollisionBulletEnemyAll();

        bool CheckCollisionCamera();

        void Update(float deltaTimeSeconds) override;

        //RenderMesh fara culoare
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        //RenderMesh culoare
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);
        //RenderMesh explozie
        void RenderMeshExplosion(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, float deltaTimeSeconds);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        glm::vec3 lightPosition;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        glm::mat4 modelMatrix, visMatrix;

        //Map Grid
        int Map_Grid_X, Map_Grid_Y;

        //Maze
        int maze[32][32];

        //Vector cu obstacolele
        std::vector <ObsCoordonate> obsCoord;
        std::vector <PlayerCoordonate> playerCoord;
        std::vector <Enemies> enemies;
        std::vector <Enemies> DeadEnemies;


        //Enemy
        float enemyScale = 0.3;
        float enemySpeed = 1.3;

        //Player
        float playerScale = 0.08;
        float player_x = 1;
        float player_y = playerScale * (2/2 + 2);
        float player_z = 1;
        float playerAngle = 0;
        float playerAngleOY = 0;
        int life = 5;

        //Tinta
        float tintaScale = 0.05f;

        //Vector cu proiectile
        std::vector <Bullet> bullets;
        float bulletMaxDist = 0.25;

        //variabile de timp pentru fire rate
        std::chrono::time_point<std::chrono::system_clock> toldFire;
        std::chrono::time_point<std::chrono::system_clock> tnewFire;

        //Camera
        implementedCamera:: Camera *camera;
        glm::mat4 projectionMatrix;
        bool cameraSwitch = false;

        float time = 2.f;

        //HUD-----------------------------------
        LogicSpace logicSpace;
        ViewportSpace viewSpace;

        float hudWidth = 3;
        float hudHeight = 1;

        int timeHud = 10000;
        int timeNumber = timeHud / 5;
      
        int lifesNumber = life;
        
        //Coordonatele de start pentru healthbar si timebar
        float healthbarX = 0.5f;
        float healthbarY = 0.5f;
        float timeX = 0.5f;
        float timeY = 2.5;
    };
}   // namespace m1
