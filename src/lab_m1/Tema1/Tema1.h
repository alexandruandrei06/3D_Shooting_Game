#pragma once

#include "components/simple_scene.h"
#include <vector>
#include <time.h>
#include <chrono>


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
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

        //Structura folosita pentru retinerea informatiilor despre un obstacol
        struct ObsCoordonate
        {
            ObsCoordonate() : minX(0), maxX(0), minY(0), maxY(0), scaleX(0), scaleY(0), angle(0) {}
            ObsCoordonate(float x, float y, float width, float height, float scaleX, float scaleY, float angle)
                : minX(x), maxX(y), minY(width), maxY(height), scaleX(scaleX), scaleY(scaleY), angle(angle){}
            float minX;
            float maxX;
            float minY;
            float maxY;
            float scaleX;
            float scaleY;
            float angle;
        };

        //Structura folosita pentru retinerea informatiilor despre un proiectil
        struct Bullet
        {
            Bullet() : X(0), Y(0), angle(0), dist(0) {}
            Bullet(float x, float y, float angle, float dist)
                : X(x), Y(y), angle(angle), dist(dist) {}
            float X;
            float Y;
            float angle;
            float dist;
        };

        //Structura folosita pentru retinerea informatiilor despre un inamic
        struct Enemies
        {
            Enemies() : X(0), Y(0), speed(0) {}
            Enemies(float x, float y, float speed)
                : X(x), Y(y), speed(speed), body("enemyBody"), arms("enemyArms"){}
            float X;
            float Y;
            float speed;
            std::string body;
            std::string arms;
        };

        //Structura folosita pentru retinerea informatiilor despre jucator
        struct PlayerS
        {
            PlayerS() {}
            PlayerS(float x, float y, float scaleX, float scaleY, float angle)
                : X(x), Y(y), angle(angle), scaleX(scaleX) , scaleY(scaleY) {}
            float X;
            float Y;
            float scaleX;
            float scaleY;
            float angle;
            std::vector<std::string> name;
        };



    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        /*Functie care adauga datele despre obstacolele si marginile hartii in vectorii respectiv fiecareia
         pentru a le utiliza la desenare si la coliziuni*/
        void AddObstacole();

        /*Functie pentru adaugarea unui nou inamic la un interval de 3 secunde*/
        void AddEnemies();

        /*Functie pentru crearea playerului*/
        void CreatePlayer();

        /*Functie pentru crearea healtbar-ului*/
        void CreateHealtbar(float x, float y);

        /*Functie care verifica coliziunea intre player si un obstacol*/
        bool CheckCollisionPlayer(PlayerS player, ObsCoordonate obstacol);

        /*Functie care verifica coliziunea intre player mai multe obstacole*/
        bool CheckCollisionPlayerAll(PlayerS player, std::vector <ObsCoordonate> obstacole);

        /*Functie care verifica coliziunea dintre un proiectil si un inamic*/
        bool CheckCollisionBulletEnemy(Bullet bullet, Enemies enemy);
        
        /*Functie care verifica coliziunea dintre toate proiectilele si toti inamicii*/
        void CheckCollisionBulletEnemyAll(std::vector<Bullet> &bullets, std::vector<Enemies> &enemies);

        /*Functie care verifica coliziunea dintre un proiectil si un obstacol*/
        bool CheckCollisionBulletObs(Bullet bullet, ObsCoordonate obstacol);

        /*Functie care verifica coliziunea dintre toate proiectilele si toate obstacolele*/
        void CheckCollisionBulletObsAll(std::vector<Bullet> &bullets, std::vector<ObsCoordonate> &obstacole);

        /*Functie care verifica coliziunea dintre un inamic si o marginea a hartii*/
        bool CheckCollisionEnemyMap(Enemies enemy, ObsCoordonate obstacol);

        /*Functie care verifica coliziunea dintre un inamic si o marginile hartii*/
        void CheckCollisionEnemyMapAll(std::vector<Enemies> &enemies, std::vector<ObsCoordonate> obstacole);

        /*Functie care verifica coliziunea dintre player si inamic*/
        bool CheckCollisionPlayerEnemy(PlayerS player, Enemies enemy);

        /*Functie care verifica coliziunea dintre player si toti inamic*/
        void CheckCollisionPlayerEnemyAll(PlayerS player, std::vector<Enemies> &enemies);

        /*Functie pentru desenarea scenei*/
        void DrawScene(glm::mat3 visMatrix);

        /*Functie pentru desenarea jucatorului*/
        void DrawPlayer(glm::mat3 visMatrix);

        /*Functie pentru desenarea Healtbar-ului*/
        void DrawHealtbar(glm::mat3 visMatrix);

        /*Functie pentru desenarea inamicilor*/
        void DrawEnemies(glm::mat3 visMatrix);

        /*Functie pentru desenarea proiectilelor*/
        void DrawBullets(glm::mat3 visMatrix);

        /*Functie pentru desenarea hartii*/
        void DrawMap(glm::mat3 visMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;

        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

    protected:
        float length;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;

        //Jucatorul
        float mouseAngle = 0;
        int speed = 5;
        int lifesNumber = 6;
        int life = lifesNumber;

        //Harta
        int mapX = 30, mapY = 22;
        float marginWidth = 0.35f;

        //Proiectile
        float bulletSize = 1.3;
        float bulletScaleX = 0.18, bulletScaleY = 0.2;
        float bulletMaxDist = 0.3;

        //Inamici
        float enemyX, enemyY;
        float enemyScaleX = 0.7, enemyScaleY = 0.7;
        float enemyAngle = 0;
        int maxGenX = 27, minGenX = 9;
        int maxGenY = 19, minGenY = 5;

        //Healtbar
        float healtbarX, healtbarY;
        float healtbarWidth = 3;
        float healtbarHeight = 1;

        //Scor
        int score = 0;
        PlayerS player;
        PlayerS futurePlayer = PlayerS(0, 0, 0, 0, 0);

        //Vector cu obstacolele
        std::vector <ObsCoordonate> obsCoord;

        //Vector cu marginile hartei
        std::vector <ObsCoordonate> mapCoord;
        
        //Vector cu proiectile
        std::vector <Bullet> bullets;

        //Vector cu inamici
        std::vector <Enemies> enemies;

        std::chrono::time_point<std::chrono::system_clock> told;
        std::chrono::time_point<std::chrono::system_clock> tnew;

        std::chrono::time_point<std::chrono::system_clock> toldEnemy;
        std::chrono::time_point<std::chrono::system_clock> tnewEnemy;
    };
}   // namespace m1
