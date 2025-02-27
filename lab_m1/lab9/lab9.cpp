#include "lab_m1/lab9/lab9.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;

Lab9::Lab9() {
    heightScale = 0.5f;
}

Lab9::~Lab9() {}

void Lab9::Init() {
    // Set projection matrix
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    // Load terrain mesh
    {
        terrain = new Mesh("terrain");
        terrain->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "Primitives"), "plane256.obj");
    }

    // Load textures
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab9", "textures");

    {
        heightMap = new Texture2D();
        heightMap->Load2D(PATH_JOIN(sourceTextureDir, "heightmap256.png").c_str(), GL_REPEAT);
    }

    {
        texture1 = new Texture2D();
        texture1->Load2D(PATH_JOIN(sourceTextureDir, "grass_bilboard.png").c_str(), GL_REPEAT);
    }

    {
        texture2 = new Texture2D();
        texture2->Load2D(PATH_JOIN(sourceTextureDir, "crate.jpg").c_str(), GL_REPEAT);
    }

    // Create shader
    {
        Shader* shader = new Shader("TerrainShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab9", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab9", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Set camera position
    GetSceneCamera()->SetPosition(glm::vec3(0, 5, 10));
    GetSceneCamera()->Update();
}

void Lab9::FrameStart() {
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Lab9::Update(float deltaTimeSeconds) {
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -2, -5));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));

    viewMatrix = GetSceneCamera()->GetViewMatrix();

    Shader* shader = shaders["TerrainShader"];
    shader->Use();


    glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(shader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(shader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));


    glUniform1f(shader->GetUniformLocation("heightScale"), heightScale);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, heightMap->GetTextureID());
    glUniform1i(shader->GetUniformLocation("heightMap"), 0);



    RenderMesh(terrain, shader, modelMatrix);


}

void Lab9::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) {
    if (!mesh || !shader || !shader->GetProgramID()) return;

    glUseProgram(shader->program);

    GLint loc_model_matrix = shader->GetUniformLocation("Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Lab9::FrameEnd() {
    DrawCoordinateSystem(viewMatrix, projectionMatrix);
}


void Lab9::OnKeyPress(int key, int mods) {}
void Lab9::OnKeyRelease(int key, int mods) {}
void Lab9::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}
void Lab9::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void Lab9::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Lab9::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void Lab9::OnWindowResize(int width, int height) {}