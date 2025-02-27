#include "lab_m1/lab3/lab3.h"
#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>



using namespace std;
using namespace m1;

Lab3::Lab3() {
    // Constructor
}

Lab3::~Lab3() {
    // Destructor
}

void Lab3::Init() {
    // Set camera
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // variabile pentru bara de viata a tankului
    tankHealth = 100.0f; // Viața tancului, inițial 100%
    maxTankHealth = 100.0f; // Valoarea maximă a vieții tancului

    tank2Health = 100.0f; // Viața inițială a tancului 2
    maxTank2Health = 100.0f; // Valoarea maximă a vieții tancului 2


    // Terrain generation
    generateTerrain(resolution);


    glLineWidth(3.0f); // Setează lățimea liniei la 3 pixeli 

    createTrapezoid("tankBase", glm::vec3(0.3f, 0.5f, 0.2f), 30.0f, 80.0f);  // Culoare verde închis

    // Creează al doilea trapez (partea de sus a tancului)
    createTrapezoid("tankTurret", glm::vec3(0.4f, 0.7f, 0.3f), 110.0f, 40.0f);  // Culoare verde deschis

    createArc("tankTurretTop", glm::vec3(0.4f, 0.7f, 0.3f), 25.0f, M_PI, 2 * M_PI, 30); // Arcul de cerc
    createRectangle("tankGun", glm::vec3(0.0f, 0.0f, 0.0f), 10.0f, 40.0f);


    createCircle("projectile", glm::vec3(1.0f, 0.0f, 0.0f), 5.0f, 20); // Proiectilul 1
    createCircle("projectile2", glm::vec3(1.0f, 0.0f, 0.0f), 5.0f, 20); // Proiectilul 2


    // bara de viata tank
    // Creează mesh pentru dreptunghiul wireframe (cadru)
    createRectangle("healthBarFrame", glm::vec3(1.0f, 1.0f, 1.0f), 60.0f, 10.0f);

    // Creează mesh pentru dreptunghiul umplut
    createRectangle("healthBarFill", glm::vec3(0.0f, 1.0f, 0.0f), 60.0f, 10.0f);


    // AL DOILEA TANK
    // Creează baza tancului al doilea tanc (culoare albastră)
    createTrapezoid("tankBase2", glm::vec3(0.2f, 0.3f, 0.8f), 30.0f, 80.0f);

    // Creează turela tancului al doilea (culoare albastru deschis)
    createTrapezoid("tankTurret2", glm::vec3(0.3f, 0.5f, 0.9f), 110.0f, 40.0f);

    // Creează arcul de cerc pentru turela superioară
    createArc("tankTurretTop2", glm::vec3(0.3f, 0.5f, 0.9f), 25.0f, M_PI, 2 * M_PI, 30);

    // Creează țeava tunului (negru - rămâne comună)
    createRectangle("tankGun2", glm::vec3(0.0f, 0.0f, 0.0f), 10.0f, 40.0f);


    // Inițializează matricea de modelare pentru tanc
    tankModelMatrix1 = glm::mat3(1);
    tankModelMatrix2 = glm::mat3(1);


}

void Lab3::RenderHealthBar(glm::mat3 tankModelMatrix, float health, float maxHealth) {
    // Dimensiunile barei de viață
    float barWidth = 60.0f;       // Lățimea completă a barei de viață
    float barHeight = 10.0f;      // Înălțimea barei
    int numSegments = 10;         // Numărul de segmente din bara de viață
    float segmentWidth = barWidth / numSegments; // Lățimea fiecărui segment

    // Calculăm numărul de segmente active în funcție de sănătate
    int activeSegments = static_cast<int>((health / maxHealth) * numSegments);

    // Matricea pentru wireframe-ul barei
    glm::mat3 wireframeModelMatrix = tankModelMatrix;
    wireframeModelMatrix *= transform2D::Translate(-barWidth / 2.0f, 69.0f); // Poziționăm bara deasupra tancului

    

    // Renderizăm fiecare segment al barei
    for (int i = 0; i < numSegments; ++i) {
        glm::mat3 segmentMatrix = tankModelMatrix;
        segmentMatrix *= transform2D::Translate(-barWidth / 2.0f + i * segmentWidth, 69.0f); // Poziționăm segmentul
        if (i < activeSegments) {
            // Renderizăm segmentul umplut (verde)
            RenderMesh2D(meshes["healthBarFill"], shaders["VertexColor"], segmentMatrix);
        }
        else {
            // Renderizăm segmentul gol (opțional)
            RenderMesh2D(meshes["healthBarEmpty"], shaders["VertexColor"], segmentMatrix);
        }
    }
}




void Lab3::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
   
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);


    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    unsigned int IBO = 0;
    // TODO(student): Create the IBO and bind it
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    // TODO(student): Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data. It will be learned later, when GLSL shaders will be
    // introduced. For the moment, just think that each property value from
    // our vertex format needs to be sent to a certain channel, in order to
    // know how to receive it in the GLSL vertex shader.

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

   

    // Check for OpenGL errors
    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
        cout << "\t[NOTE] : For developers : This happens because OpenGL core spec >=3.1 forbids null VAOs." << std::endl;
    }

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}

Mesh* Lab3::createArc(const std::string& name, glm::vec3 color, float radius, float startAngle, float endAngle, int numSegments) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Adaugă centrul arcului
    vertices.emplace_back(glm::vec3(0, 0, 0), color);

    float angleStep = (endAngle - startAngle) / numSegments;

    // Adaugă punctele de pe marginea arcului
    for (int i = 0; i <= numSegments; ++i) {
        float angle = startAngle + i * angleStep;
        vertices.emplace_back(glm::vec3(radius * cos(angle), radius * sin(angle), 0), color);
    }

    // Creează triunghiuri între punctele de pe arc și centrul
    for (int i = 1; i <= numSegments; ++i) {
        indices.push_back(0);      // Centrul
        indices.push_back(i);      // Punct curent
        indices.push_back(i + 1);  // Punct următor
    }

    // Creează mesh-ul
    Mesh* arcMesh = new Mesh(name);
    arcMesh->InitFromData(vertices, indices);
    meshes[name] = arcMesh;
    AddMeshToList(arcMesh);

    return arcMesh;
}


Mesh* Lab3::createRectangle(const std::string& name, glm::vec3 color, float width, float height) {
    std::vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(0, 0, 0), color),
        VertexFormat(glm::vec3(width, 0, 0), color),
        VertexFormat(glm::vec3(width, height, 0), color),
        VertexFormat(glm::vec3(0, height, 0), color)
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };

    Mesh* rectangleMesh = new Mesh(name);
    rectangleMesh->InitFromData(vertices, indices);
    meshes[name] = rectangleMesh;
    AddMeshToList(rectangleMesh);

    return rectangleMesh;
}


Mesh* Lab3::createCircle(const std::string& name, glm::vec3 color, float radius, int numSegments) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Adaugă centrul cercului
    vertices.emplace_back(glm::vec3(0, 0, 0), color);

    // Adaugă punctele de pe marginea cercului
    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * 2 * M_PI / numSegments;
        vertices.emplace_back(glm::vec3(radius * cos(angle), radius * sin(angle), 0), color);
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    Mesh* circleMesh = new Mesh(name);
    circleMesh->InitFromData(vertices, indices);
    meshes[name] = circleMesh;
    AddMeshToList(circleMesh);

    return circleMesh;
}



Mesh* Lab3::createTrapezoid(const std::string& name, glm::vec3 color, float baseWidth, float topWidth) {

    float height = 20.0f;     // Înălțimea trapezului

    std::vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(-baseWidth / 2, 0, 0), color),
        VertexFormat(glm::vec3(baseWidth / 2, 0, 0), color),
        VertexFormat(glm::vec3(-topWidth / 2, height, 0), color),
        VertexFormat(glm::vec3(topWidth / 2, height, 0), color)
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 2, 1, 3 };

    Mesh* trapezoidMesh = new Mesh(name);
    trapezoidMesh->InitFromData(vertices, indices);
    meshes[name] = trapezoidMesh;
    AddMeshToList(trapezoidMesh);

    return trapezoidMesh;
}



void Lab3::generateTerrain(glm::ivec2 resolution) {
    terrainPoints.clear();

    const int numPoints = 500;   // Numărul de puncte dorit
    const float terrainHeight = 10; // Înălțime maximă micșorată
    const float yOffset = 350.0f;   // Baza terenului coborâtă
    const float frequency1 = 0.015f;
    const float frequency2 = 0.03f;

    // Calculăm pasul între puncte
    float step = (float)resolution.x * 2 / (numPoints - 1);

    for (int i = 0; i < numPoints; ++i) {
        float x = i * step;
        float y = (terrainHeight / 2) * sin(frequency1 * x) +
            30 * sin(frequency2 * x) + // Amplitudine redusă
            yOffset;
        terrainPoints.push_back(glm::vec3(x, y, 0));
    }

    buildTerrainMesh(terrainPoints);
}





void Lab3::buildTerrainMesh(const std::vector<glm::vec3>& terrainPoints) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;
    glm::vec3 terrainColor(1.0f, 0.4f, 0.7f);

    for (size_t i = 0; i < terrainPoints.size(); ++i) {
        vertices.emplace_back(glm::vec3(terrainPoints[i].x, 0, 0), terrainColor); // Punct jos
        vertices.emplace_back(glm::vec3(terrainPoints[i]), terrainColor);         // Punct sus
        indices.push_back(i * 2);
        indices.push_back(i * 2 + 1);
    }

    Mesh* terrainMesh = new Mesh("terrain");
    terrainMesh->InitFromData(vertices, indices);
    terrainMesh->SetDrawMode(GL_TRIANGLE_STRIP);
    AddMeshToList(terrainMesh);
}


void Lab3::FrameStart() {
    glClearColor(208.0f / 255.0f, 235.0f / 255.0f, 255.0f / 255.0f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


float Lab3::GetTerrainHeight(float tankX) {
    if (terrainPoints.empty()) return 0.0f;

    // Asigură-te că tankX este în intervalul valid
    if (tankX < terrainPoints.front().x || tankX > terrainPoints.back().x) {
        return 0.0f; // În afara terenului
    }

    // Găsește segmentul dintre două puncte între care se află tankX
    for (size_t i = 0; i < terrainPoints.size() - 1; ++i) {
        if (terrainPoints[i].x <= tankX && tankX <= terrainPoints[i + 1].x) {
            float t = (tankX - terrainPoints[i].x) / (terrainPoints[i + 1].x - terrainPoints[i].x);
            return glm::mix(terrainPoints[i].y, terrainPoints[i + 1].y, t);
        }
    }

    return 0.0f; // Fallback
}




void Lab3::PlaceAndOrientTank(float tankX) {
    // Obține poziția pe Y a tancului pe teren (punctul A)
    float tankY = GetTerrainHeight(tankX);

    // Definim deltaX pentru calculul pantei
    float deltaX = 1.0f;

    // Obținem poziția pe Y la punctul B
    float nextY = GetTerrainHeight(tankX + deltaX);

    // Calculăm deltaY
    float deltaY = nextY - tankY;

    // Calculăm unghiul de rotație în funcție de panta terenului
    float angle = atan2(deltaY, deltaX);
    angle = -angle;


    // Construim matricea de modelare pentru tanc
    tankModelMatrix1 = glm::mat3(1);

    

    // Aplicăm transformările: mai întâi translația, apoi rotația
    tankModelMatrix1 *= transform2D::Translate(tankX, tankY);
    tankModelMatrix1 *= transform2D::Rotate(angle);
}

void Lab3::PlaceAndOrientTank2(float tankX) {
    // Obține poziția pe Y pentru tancul al doilea
    float tankY = GetTerrainHeight(tankX);

    // Calculăm panta terenului și rotația
    float deltaX = 1.0f;
    float nextY = GetTerrainHeight(tankX + deltaX);
    float deltaY = nextY - tankY;
    float angle = -atan2(deltaY, deltaX);

  

    // Construim matricea de modelare
    tankModelMatrix2 = glm::mat3(1);
    tankModelMatrix2 *= transform2D::Translate(tankX, tankY);
    tankModelMatrix2 *= transform2D::Rotate(angle);
}


std::vector<glm::vec2> Lab3::CalculateTrajectory(float angle, float initialSpeed, glm::vec2 startPosition) {
    std::vector<glm::vec2> trajectoryPoints;

    const float timeStep = 0.005f; // Pas de timp mai mic pentru precizie
    float time = 0.0f;

    // Componentele vitezei inițiale
    float vx = initialSpeed * cos(angle);
    float vy = initialSpeed * sin(angle);

    while (true) {
        // Calculăm poziția la momentul t
        float x = startPosition.x + vx * time;
        float y = startPosition.y + vy * time - 0.5f * gravity * time * time;

        // Verificăm dacă proiectilul a atins terenul sau a ieșit din ecran
        if (y < 0 || x < 0 || x > window->GetResolution().x) {
            break;
        }

        // Verificăm înălțimea terenului la poziția x
        float terrainHeight = GetTerrainHeight(x);
        if (y <= terrainHeight) {
            y = terrainHeight;
            trajectoryPoints.push_back(glm::vec2(x, y));
            break;
        }

        // Adăugăm punctul în traiectorie
        trajectoryPoints.push_back(glm::vec2(x, y));

        // Actualizăm timpul
        time += timeStep;
    }

    return trajectoryPoints;
}




float Lab3::ComputeTankXPosition(float deltaTimeSeconds) {
    static float totalTime = 0.0f;
    totalTime += deltaTimeSeconds;
    float screenWidth = window->GetResolution().x;
    // Oscilație sinusoidală între marginea stângă și dreaptă a ecranului
    return (screenWidth / 2) * (1 + sin(totalTime * 0.5f));
}

float Lab3::UpdateTankPosition(float deltaTimeSeconds) {
    static float tankX = 400.0f; // Poziția inițială a tancului
    static float speed = 150.0f; // Viteza de deplasare
    glm::ivec2 resolution = window->GetResolution(); // Dimensiunea cadrului

    if (window->KeyHold(GLFW_KEY_ENTER)) {
        if (!isKeyPressed) { // Apăsarea inițială
            isKeyPressed = true;
            CreateProjectile(); // Creează proiectilul
        }
    }
    else {
        // Resetăm starea când tasta nu mai este apăsată
        isKeyPressed = false;
    }
    // Actualizează poziția pe baza tastelor
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        tankX -= speed * deltaTimeSeconds;
        // Aplică constrângeri pentru limitele cadrului
        if (tankX < 30) {
            tankX = 30;
        }
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        tankX += speed * deltaTimeSeconds;
        if (tankX > resolution.x) {
            tankX = resolution.x; // Limitează la marginea dreaptă
        }
    }
    if (window->KeyHold(GLFW_KEY_UP)) {
        gunAngle += 1.5f * deltaTimeSeconds; // Crește unghiul
        if (gunAngle > M_PI / 2) gunAngle = M_PI / 2; // Limitare la 90°
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        gunAngle -= 1.5f * deltaTimeSeconds; // Scade unghiul
        if (gunAngle < 0) gunAngle = 0; // Limitare la 0°
    }
   

    // Actualizează poziția pe Y în funcție de teren
    float tankY = GetTerrainHeight(tankX);

    // Întoarce poziția actualizată pe X (în funcție de constrângeri)
    return tankX;
}



float Lab3::UpdateTank2Position(float deltaTimeSeconds) {
    static float tankX2 = 600.0f; // Poziția inițială pentru tancul 2
    static float speed = 150.0f;  // Viteza de deplasare
    glm::ivec2 resolution = window->GetResolution(); // Dimensiunea cadrului

    if (window->KeyHold(GLFW_KEY_SPACE)) {
        if (!isKeyPressed2) { // Apăsarea inițială
            isKeyPressed2 = true;
            CreateProjectile2(); // Creează proiectilul
        }
    }
    else {
        // Resetăm starea când tasta nu mai este apăsată
        isKeyPressed2 = false;
    }
    // Controlează mișcarea cu tastele F și G
    if (window->KeyHold(GLFW_KEY_A)) {
        tankX2 -= speed * deltaTimeSeconds;
        if (tankX2 < 50) {
            tankX2 = 50; // Limitează la marginea stângă
        }
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        tankX2 += speed * deltaTimeSeconds;
        if (tankX2 > resolution.x) {
            tankX2 = resolution.x; // Limitează la marginea dreaptă
        }
    }
    if (window->KeyHold(GLFW_KEY_W)) {
    gunAngle2 += 1.5f * deltaTimeSeconds; // Crește unghiul turelei
    if (gunAngle2 > M_PI / 2) gunAngle2 = M_PI / 2; // Limitare la +90° (spre dreapta)
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        gunAngle2 -= 1.5f * deltaTimeSeconds; // Scade unghiul turelei
        if (gunAngle2 < -M_PI / 2) gunAngle2 = -M_PI / 2; // Limitare la -90° (spre stânga)
    }



    // Întoarce poziția actualizată pe X
    return tankX2;
}

void Lab3::CheckAndRemoveTank(float& tankHealth, bool& isTankDestroyed) {
    if (tankHealth <= 0.0f) {
        isTankDestroyed = true;
    }
}

void Lab3::RenderTank1(float deltaTimeSeconds) {
    // Calculăm poziția tancului 1
    tankX = UpdateTankPosition(deltaTimeSeconds);
    tankY = GetTerrainHeight(tankX);

    // Plasează și orientează tancul 1
    PlaceAndOrientTank(tankX);

    // Verifică dacă tancul 1 este distrus
    if (!isTank1Destroyed) {
        // Randează baza tancului
        RenderMesh2D(meshes["tankBase"], shaders["VertexColor"], tankModelMatrix1);

        // Randează turela tancului
        glm::mat3 turretModelMatrix = tankModelMatrix1;
        turretModelMatrix *= transform2D::Translate(0, 20); // Poziție relativă
        RenderMesh2D(meshes["tankTurret"], shaders["VertexColor"], turretModelMatrix);

        // Randează arcul de cerc (partea superioară a turelei)
        glm::mat3 turretTopMatrix = turretModelMatrix;
        turretTopMatrix *= transform2D::Translate(0, 10.0f);
        turretTopMatrix *= transform2D::Rotate(M_PI); // Rotire pentru partea de sus
        RenderMesh2D(meshes["tankTurretTop"], shaders["VertexColor"], turretTopMatrix);

        // Randează țeava tunului
        glm::mat3 gunMatrix = turretModelMatrix;
        gunMatrix *= transform2D::Translate(0, 20); // Poziție relativă
        gunMatrix *= transform2D::Rotate(gunAngle); // Rotirea țeavii
        RenderMesh2D(meshes["tankGun"], shaders["VertexColor"], gunMatrix);

        // Randează bara de viață
        RenderHealthBar(tankModelMatrix1, tankHealth, maxTankHealth);

        // Randează traiectoria proiectilului
        float totalGunAngle = tankRotationAngle + gunAngle;
        glm::vec3 gunTipLocal(0, 40.0f, 1); // Lungimea țeavii
        glm::vec3 gunTipWorld = gunMatrix * gunTipLocal;
        glm::vec2 startPosition = glm::vec2(gunTipWorld.x, gunTipWorld.y);
        std::vector<glm::vec2> trajectoryPoints = CalculateTrajectory(totalGunAngle, 500.0f, startPosition);

        Mesh* trajectoryMesh = CreateTrajectoryMesh("trajectory", trajectoryPoints, glm::vec3(1.0f, 1.0f, 1.0f));
        RenderMesh2D(trajectoryMesh, shaders["VertexColor"], glm::mat3(1));
    }
}

void Lab3::RenderTank2(float deltaTimeSeconds) {
    // Calculăm poziția tancului 2
    tank2X = UpdateTank2Position(deltaTimeSeconds);
    tank2Y = GetTerrainHeight(tank2X);

    // Plasează și orientează tancul 2
    PlaceAndOrientTank2(tank2X);

    // Verifică dacă tancul 2 este distrus
    if (!isTank2Destroyed) {
        // Randează baza tancului 2
        RenderMesh2D(meshes["tankBase2"], shaders["VertexColor"], tankModelMatrix2);

        // Randează turela tancului al doilea
        glm::mat3 turretModelMatrix2 = tankModelMatrix2;
        turretModelMatrix2 *= transform2D::Translate(0, 20);
        RenderMesh2D(meshes["tankTurret2"], shaders["VertexColor"], turretModelMatrix2);

        // Randează arcul de cerc al turelei al doilea tanc
        glm::mat3 turretTopMatrix2 = turretModelMatrix2;
        turretTopMatrix2 *= transform2D::Translate(0, 10.0f);
        turretTopMatrix2 *= transform2D::Rotate(M_PI);
        RenderMesh2D(meshes["tankTurretTop2"], shaders["VertexColor"], turretTopMatrix2);

        // Randează țeava tunului al doilea
        glm::mat3 gunMatrix2 = turretModelMatrix2;
        gunMatrix2 *= transform2D::Translate(0, 20);
        gunMatrix2 *= transform2D::Rotate(gunAngle2);
        RenderMesh2D(meshes["tankGun2"], shaders["VertexColor"], gunMatrix2);

        // Randează bara de viață
        RenderHealthBar(tankModelMatrix2, tank2Health, maxTank2Health);

        // Randează traiectoria proiectilului
        totalGunAngle2 = M_PI + (tankRotationAngle2 + gunAngle2);
        glm::vec3 gunTipLocal2(0, 40.0f, 1); // Lungimea țeavii
        glm::vec3 gunTipWorld2 = gunMatrix2 * gunTipLocal2;
        glm::vec2 startPosition2 = glm::vec2(gunTipWorld2.x, gunTipWorld2.y);
        std::vector<glm::vec2> trajectoryPoints2 = CalculateTrajectory(totalGunAngle2, 500.0f, startPosition2);

        Mesh* trajectoryMesh2 = CreateTrajectoryMesh("trajectory2", trajectoryPoints2, glm::vec3(1.0f, 1.0f, 1.0f));
        RenderMesh2D(trajectoryMesh2, shaders["VertexColor"], glm::mat3(1));

        // Actualizează proiectilele tancului 2
        UpdateProjectiles2(deltaTimeSeconds);
    }
}

void Lab3::Update(float deltaTimeSeconds) {
    // Randează terenul
    RenderMesh2D(meshes["terrain"], shaders["VertexColor"], glm::mat3(1));

    // Randează tancul 1
    RenderTank1(deltaTimeSeconds);

    // Randează tancul 2
    RenderTank2(deltaTimeSeconds);

    // Actualizează proiectilele
    UpdateProjectiles(deltaTimeSeconds);
    UpdateProjectiles2(deltaTimeSeconds);

}





void Lab3::DeformTerrain(float impactX, float radius, float deformationDepth) {
    for (size_t i = 0; i < terrainPoints.size(); ++i) {
        float distance = abs(terrainPoints[i].x - impactX);

        if (distance <= radius) {
            // Aplică deformarea cu atenuare
            float falloff = (radius - distance) / radius;
            terrainPoints[i].y -= deformationDepth * falloff;

            // Limitează deformarea la 0 (baza terenului)
            terrainPoints[i].y = max(terrainPoints[i].y, 0.0f);
        }
    }

    // Reconstruiește mesh-ul
    buildTerrainMesh(terrainPoints);
}



bool Lab3::CheckCollisionWithTerrain(const Projectile& projectile, float collisionThreshold) {
    float x = projectile.position.x;
    float y = projectile.position.y;

    // Găsește segmentul relevant pentru `x`
    for (size_t i = 0; i < terrainPoints.size() - 1; ++i) {
        if (terrainPoints[i].x <= x && x <= terrainPoints[i + 1].x) {
            // Interpolare pentru înălțimea terenului
            float t = (x - terrainPoints[i].x) / (terrainPoints[i + 1].x - terrainPoints[i].x);
            float terrainY = glm::mix(terrainPoints[i].y, terrainPoints[i + 1].y, t);

            // Verifică coliziunea
            return abs(y - terrainY) <= collisionThreshold;
        }
    }

    return false;
}


// functie care determina intersectia unui tank cu alt tank
bool Lab3::CheckCollisionWithTank(const Projectile& projectile, const glm::mat3& tankModelMatrix, float collisionRadius, bool isTankDestroyed) {
    if (isTankDestroyed) {
        return false;
    }

    glm::vec2 tankPosition = glm::vec2(tankModelMatrix[2][0], tankModelMatrix[2][1]);

    float distance = glm::length(projectile.position - tankPosition);

    return distance <= collisionRadius;
}


void Lab3::UpdateProjectiles(float deltaTimeSeconds) {
    for (size_t i = 0; i < projectiles.size();) {
        Projectile& proj = projectiles[i];

        // Actualizează poziția
        proj.position += proj.velocity * deltaTimeSeconds;

        // Aplică gravitația asupra componentei y a vitezei
        proj.velocity.y -= gravity * deltaTimeSeconds;

        // Renderizează proiectilul
        glm::mat3 modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(proj.position.x, proj.position.y);
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);

        // Verifică coliziunea cu tancul inamic (tancul 2)
        if (CheckCollisionWithTank(proj, tankModelMatrix2, 30.0f, isTank2Destroyed)) { // Raza de coliziune = 30.0f
            std::cout << "Coliziune detectată cu tancul 2!" << std::endl;

            // Reduce sănătatea tancului inamic
            tank2Health -= 20.0f;
            if (tank2Health <= 0.0f) {
                isTank2Destroyed = true;
               
            }

            // Elimină proiectilul
            projectiles.erase(projectiles.begin() + i);
            continue; // Sari peste incrementare
        }

        // Verifică coliziunea
        if (CheckCollisionWithTerrain(proj, 5.0f)) { // Prag de coliziune = 5.0f
          

            // Deformează terenul
            DeformTerrain(proj.position.x, 50.0f, 20.0f);

            // Elimină proiectilul
            projectiles.erase(projectiles.begin() + i);
            continue; // Sari peste incrementare
        }



        // Verifică dacă proiectilul a ieșit din ecran
        if (proj.position.y < 0 || proj.position.x < 0 || proj.position.x > window->GetResolution().x) {
           

            projectiles.erase(projectiles.begin() + i);
            continue; // Sari peste incrementare
        }

        // Incrementăm doar dacă proiectilul nu a fost șters
        ++i;
    }
}

void Lab3::UpdateProjectiles2(float deltaTimeSeconds) {
    
    for (size_t i = 0; i < projectiles2.size();) {
        Projectile& proj = projectiles2[i];

        // Actualizează poziția
        proj.position += proj.velocity * deltaTimeSeconds;

        // Aplică gravitația asupra componentei y a vitezei
        proj.velocity.y -= gravity * deltaTimeSeconds;


        // Renderizează proiectilul
        glm::mat3 modelMatrix2 = glm::mat3(1);
        modelMatrix2 *= transform2D::Translate(proj.position.x, proj.position.y);
        RenderMesh2D(meshes["projectile2"], shaders["VertexColor"], modelMatrix2);

        if (CheckCollisionWithTank(proj, tankModelMatrix1, 30.0f, isTank1Destroyed)) { // Raza de coliziune = 30.0f

            // Reduce sănătatea tancului inamic
            tankHealth -= 20.0f;
            if (tankHealth <= 0.0f) {
                isTank1Destroyed = true;
                
            }

            // Elimină proiectilul
            projectiles2.erase(projectiles2.begin() + i);
            continue; // Sari peste incrementare
        }
     
        // Verifică coliziunea cu terenul
        if (CheckCollisionWithTerrain(proj, 5.0f)) { // Prag de coliziune = 5.0f
          
            DeformTerrain(proj.position.x, 50.0f, 20.0f);
            projectiles2.erase(projectiles2.begin() + i); // Corect
            continue; // Sari peste incrementare
        }

        // Verifică dacă proiectilul a ieșit din ecran
        if (proj.position.y < 0 || proj.position.x < 0 || proj.position.x > window->GetResolution().x) {
          
            projectiles2.erase(projectiles2.begin() + i); // Corect
            continue; // Sari peste incrementare
        }

        // Incrementăm doar dacă proiectilul nu a fost șters
        ++i;
    }
}





Mesh* Lab3::CreateTrajectoryMesh(const std::string& name, const std::vector<glm::vec2>& points, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < points.size(); ++i) {
        vertices.emplace_back(glm::vec3(points[i], 0), color);
        indices.push_back(i);
    }

    Mesh* trajectoryMesh;
    if (meshes.find(name) != meshes.end()) {
        // Dacă mesh-ul există deja, actualizează-l
        trajectoryMesh = meshes[name];
        trajectoryMesh->ClearData();
        trajectoryMesh->InitFromData(vertices, indices);
    }
    else {
        // Dacă mesh-ul nu există, creează-l
        trajectoryMesh = new Mesh(name);
        trajectoryMesh->SetDrawMode(GL_LINE_STRIP);
        trajectoryMesh->InitFromData(vertices, indices);
        meshes[name] = trajectoryMesh;
        AddMeshToList(trajectoryMesh);
    }

    return trajectoryMesh;
}



void Lab3::CreateProjectile() {
    // Calculăm matricea completă a țevii
    glm::mat3 gunMatrix = tankModelMatrix1;
    gunMatrix *= transform2D::Translate(0, 20); // Poziție relativă față de turela tancului
    gunMatrix *= transform2D::Rotate(gunAngle);

    // Poziția locală și globală a vârfului țevii
    glm::vec3 gunTipLocal(0, 40.0f, 1); // Lungimea țevii
    glm::vec3 gunTipWorld = gunMatrix * gunTipLocal;

    // Direcția țeavii
    glm::vec2 gunDirection(cos(gunAngle), sin(gunAngle));
    glm::vec2 initialVelocity = gunDirection * initialSpeed1;

    // Creăm proiectilul
    Projectile proj;
    proj.position = glm::vec2(gunTipWorld.x, gunTipWorld.y);
    proj.velocity = initialVelocity;

    // Adăugăm proiectilul în listă
    projectiles.push_back(proj);
}



void Lab3::CreateProjectile2() {
    // Calculăm matricea completă a țevii pentru tancul 2
    glm::mat3 gunMatrix2 = tankModelMatrix2;
    gunMatrix2 *= transform2D::Translate(0, 20); // Poziție relativă față de turela tancului
    
    gunMatrix2 *= transform2D::Rotate(gunAngle2); // Rotire în funcție de unghiul tunului

    // Poziția locală și globală a vârfului țevii
    glm::vec3 gunTipLocal2(0, 40.0f, 1); // Lungimea țevii
    glm::vec3 gunTipWorld2 = gunMatrix2 * gunTipLocal2;

  
    glm::vec2 gunDirection(cos(totalGunAngle2), sin(totalGunAngle2));

    // Viteza inițială
    float magnitude = 500.0f; // Ajustați dacă este necesar
    glm::vec2 initialVelocity = gunDirection * magnitude;

    // Creăm proiectilul
    Projectile proj;
    proj.position = glm::vec2(gunTipWorld2.x, gunTipWorld2.y);
    proj.velocity = initialVelocity;

    // Adăugăm proiectilul în listă
    projectiles2.push_back(proj);

}


void Lab3::updateObjectAnimation(const string& meshName, float posX, float posY, float deltaTime) {
    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(posX, posY);
    RenderMesh2D(meshes[meshName], shaders["VertexColor"], modelMatrix);
}

void Lab3::FrameEnd() {
    
}