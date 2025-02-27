#pragma once

#include "components/simple_scene.h"
#include <vector>
#include <glm/glm.hpp>

namespace m1 {
    class Lab3 : public gfxc::SimpleScene {
    public:
        Lab3();
        ~Lab3();

        void Init() override;
        void Update(float deltaTimeSeconds) override;
        void FrameStart() override;
        void FrameEnd() override;
        void LoadTankModel();
        void PlaceAndOrientTank(float tankX);
        void HandleTankInput(float deltaTimeSeconds, float& tankX, float& turretAngle);
        float UpdateTankPosition(float deltaTimeSeconds);
        bool hasActiveProjectile = false;
        bool isKeyPressed = false;
        bool isKeyPressed2 = false;
        // variabile care imi spun cand sterg un tank
        bool isTank1Destroyed = false;
        bool isTank2Destroyed = false;
       

        void Lab3::CheckAndRemoveTank(float& tankHealth, bool& isTankDestroyed);
        void Lab3::RenderTank1(float deltaTimeSeconds);
        void Lab3::RenderTank2(float deltaTimeSeconds);

    private:
        struct Projectile {
            glm::vec2 position;
            glm::vec2 velocity;
        };
        float totalGunAngle2;
        void Lab3::CreateProjectile2();
        float tankX, tankY;       // Poziția tancului 1
        float tank2X, tank2Y;     // Poziția tancului 2

        std::vector<Projectile> projectiles;  // Lista proiectilelor
        std::vector<Projectile> projectiles2;
        float gravity = 981.0f;               // Valoarea accelerației gravitaționale
        float gunAngle = M_PI / 4;            // Unghiul țeavii tunului (inițial 45 de grade)
        float tankRotationAngle = 0.0f;       // Unghiul de rotație al tancului în funcție de panta terenului
        glm::mat3 gunModelMatrix;             // Matricea de modelare pentru țeava tunului

        void CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        Mesh* createTrapezoid(glm::vec3 color);
        Mesh* createRectangle(glm::vec3 color);
        void generateTerrain(glm::ivec2 resolution);
        void buildTerrainMesh(const std::vector<glm::vec3>& terrainPoints);
        float GetTerrainHeight(float x);
        Mesh* createTrapezoid(const std::string& name, glm::vec3 color, float baseWidth, float topWidth);
        Mesh* createRectangle(const std::string& name, glm::vec3 color, float radius, float startAngle, float endAngle, int numSegments);
        Mesh* createCircle(const std::string& name, glm::vec3 color, float radius, int numSegments);
        Mesh* createRectangle(const std::string& name, glm::vec3 color, float width, float height);
        Mesh* createArc(const std::string& name, glm::vec3 color, float radius, float startAngle, float endAngle, int numSegments);
        float ComputeTankXPosition(float deltaTimeSeconds);
        void updateObjectAnimation(const std::string& meshName, float posX, float posY, float deltaTime);
        void CreateProjectile();
        void Lab3::UpdateProjectiles(float deltaTimeSeconds);
        void Lab3::UpdateProjectiles2(float deltaTimeSeconds);

        Mesh* CreateTrajectoryMesh(const std::string& name, const std::vector<glm::vec2>& points, glm::vec3 color);
        std::vector<glm::vec2> CalculateTrajectory(float angle, float initialSpeed, glm::vec2 startPosition);
        bool CheckCollisionWithTerrain(const Projectile& projectile, float collisionThreshold);
        void DeformTerrain(float impactX, float radius, float deformationDepth);
        void SlideTerrain(float deltaTime, float heightThreshold, float transferRate);
        void RenderHealthBar(float tankX, float tankY, float health, float maxHealth);
        void PlaceAndOrientTank2(float tankX);
        float UpdateTank2Position(float deltaTimeSeconds);

        void RenderHealthBar(glm::mat3 tankModelMatrix, float health, float maxHealth);
        float Lab3::NormalizeAngle(float angle);

        std::vector<glm::vec3> terrainPoints;
        int numPoints;
        float width;
        float amplitude;
        float frequency;
        float phase;
        glm::mat3 tankModelMatrix1;
        glm::mat3 tankModelMatrix2;
        float tank2Health; // Viața inițială a tancului 2
        float maxTank2Health; // Valoarea maximă a vieții tancului 2
        std::vector<glm::vec2> Lab3::CalculateTrajectory2(float angle, float initialSpeed, glm::vec2 startPosition);
        // functie care determina intersectia unui tank cu alt tank
        bool Lab3::CheckCollisionWithTank(const Projectile& projectile, const glm::mat3& tankModelMatrix, float collisionRadius, bool isTankDestroyed);
 
        float tankRotationAngle1;
        float tankRotationAngle2 = 0.0f;
     
        float gunAngle2 =  - M_PI / 4;
        float initialSpeed1 = 500.0f;
        float projectileYOffset = 0.0f; // Offset vertical al proiectilului în faza de pregătire
        std::vector<glm::vec2> Lab3::CalculateTrajectoryGuide(float angle, float initialSpeed, glm::vec2 startPosition);
        Mesh* Lab3::CreateTrajectoryGuideMesh(const std::string& name, const std::vector<glm::vec2>& points, glm::vec3 color);
        float initialTank1X = 400.0f; // Poziția inițială pentru Tank 1
        float initialTank2X = 600.0f; // Poziția inițială pentru Tank 2
        
        // variabile pentru terminarea jocului 
        bool gameOver = false;
        std::string winnerText = "";
        

        // variabile ce detaliaza bara tankului
        float tankHealth;        // Sănătatea curentă a tancului
        float maxTankHealth;     // Sănătatea maximă a tancului
    };
}
