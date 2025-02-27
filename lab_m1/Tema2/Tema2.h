// #pragma once

 #include "components/simple_scene.h"
#include "lab_m1/Tema2/lab_camera.h"


namespace m1
{
 	class Tema2 : public gfxc::SimpleScene
 	{
	public:
 		Tema2();
 		~Tema2();

 		void Init() override;

 	private:
        // variabile pentru timer
        float gameTimer = 120.0f;        
        bool timerExpired = false;       
        bool raceStarted = false;        
        bool raceFinished = false;       
        int currentGateIndex = 0;    

        Mesh* Tema2::CreateText(const std::string& name);



        Mesh* CreateSmallTerrain(const std::string& name);
        void Tema2::RenderGate(const glm::vec3& position);

        void Tema2::DrawRectangle(float x, float y, float width, float height, glm::vec3 color, float alpha);

 		void FrameStart() override;
 		void Update(float deltaTimeSeconds) override;
 		void FrameEnd() override;

		void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

 		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
 		void OnKeyRelease(int key, int mods) override;
 		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
 		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
 		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
 		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
 		void OnWindowResize(int width, int height) override;

        Mesh* CreateDroneBody(const std::string& name);
        Mesh* CreateDronePropeller(const std::string& name);
        void RenderDrone(float deltaTimeSeconds);
        Mesh* CreateTerrain(const std::string& name);
        void RenderTerrain();
        Mesh* Tema2::CreateSun(const std::string& name);

        // pentru a crea obstacolele
        Mesh* CreateObstacle(const std::string& name);
        
        // ursi ca obstacole
        Mesh* CreateBearBody(const std::string& name);
        Mesh* CreateBearHead(const std::string& name);
        Mesh* CreateBearEar(const std::string& name);
        Mesh* CreateBearLeg(const std::string& name);


        // parametrii pentru camera third-person
        float cameraDistance = 1.5f;
        float cameraHeight = 1.5f;
        float cameraLag = 0.1f;

        // am o sfera ce va aproximeaza sfera
        float droneRadius = 0.5f;

        // coleziounea cu obstacole sau cu terenul
        bool CheckCollisionWithGround();
        bool CheckCollisionWithObstacles();
        bool SphereAABBCollision(const glm::vec3& sphereCenter, float sphereRadius,
            const glm::vec3& boxMin, const glm::vec3& boxMax);
        glm::vec3 oldDronePosition;

        // detectez coleziunea cu copacii
        bool CheckCollisionWithTrees();
        const float TREE_TRUNK_RADIUS = 0.1f;
        const float TREE_HEIGHT = 2.0f;

        // parametrii pentru drona
        
        struct DronePhysics {
            float thrust = 0.0f;         
            float maxThrust = 15.0f;    
            float minThrust = 9.81f;      
            float thrustAcceleration = 10.0f;

            glm::vec3 rotation = glm::vec3(0); 
            float maxRotationAngle = glm::radians(45.0f); 
            float rotationSpeed = 2.0f;    

            glm::vec3 velocity = glm::vec3(0);
            float drag = 0.1f;           
            float gravity = -9.81f;      
        } physics;

        const std::vector<glm::vec3> propellerPositions = {
        glm::vec3(0.3f, 0.1f, 0.3f),    
        glm::vec3(-0.3f, 0.1f, 0.3f),   
        glm::vec3(0.3f, 0.1f, -0.3f),   
        glm::vec3(-0.3f, 0.1f, -0.3f)   
        };

        bool CheckGateCollision(const glm::vec3& sphereCenter, float sphereRadius, const glm::vec3& gatePosition);
        bool CheckGatePassthrough(const glm::vec3& sphereCenter, const glm::vec3& gatePosition);
        Mesh* Tema2::CreateFlower(const std::string& name);
        std::vector<glm::vec3> flowerPositions;


        float thrust = 0.0f;
        float maxThrust = 15.0f;
        float thrustAcceleration = 20.0f;
        float dragCoefficient = 0.1f;
        float gravity = -9.81f;


        glm::vec3 droneVelocity;     
        glm::vec3 droneRotation;

        // animatie ursi
        float bearTime = 0;

        // nivelul terenului
        const float GROUND_LEVEL = -2.0f;  
        const float DRONE_SIZE = 0.5f;

        int viewportWidth;   
        int viewportHeight;

        // metode prin care se livreaza pachetul
      
       

        glm::vec3 dronePosition;
        Mesh* Tema2::CreateGate(const std::string& name);
       
        std::vector<glm::vec3> gatePositions;

        
        std::vector<int> checkpointOrder;        
        int currentCheckpoint = 0;               
                  

        
        glm::vec3 arrowColor = glm::vec3(1, 1, 0);  
        float arrowRotation = 0.0f;             
        void Tema2::RenderText(const std::string& text, float x, float y, glm::vec3 color);
 	protected:

        implemented::Camera* minimapCamera;
        glm::mat4 minimapProjection;

        // dimensiunea camerei de minimap
        const float MINIMAP_SIZE = 0.1f;
        void RenderMinimap();

 		implemented::Camera* camera;
 		glm::mat4 projectionMatrix;
 		bool renderCameraTarget;
        
        float propellerRotation;

 		// TODO(student): If you need any other class variables, define them here.
 		float top, bottom, left, right;
 		float fov;
 		float znear, zfar;
 		bool perspective = true;

        std::vector<glm::vec3> treePositions;      
        std::vector<glm::vec3> obstaclePositions;  
        std::vector<glm::vec3> obstacleScales;    

        // metode necesare pentru crearea brazilor
        Mesh* CreateTreeTrunk(const std::string& name);
        Mesh* CreateTreeTop(const std::string& name);

        std::vector<glm::vec3> bearPositions;
        std::vector<float> bearRotations;


 	};
 }   // namespace m1