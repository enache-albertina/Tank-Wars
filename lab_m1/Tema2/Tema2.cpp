#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include "components/text_renderer.h"

#define PACKAGE_PICKUP_RADIUS 1.0f
#define DELIVERY_RADIUS 1.0f
#define DRONE_RADIUS 0.5f
#define TREE_TRUNK_RADIUS 0.5f

#define TREE_HEIGHT 2.0f

using namespace std;
using namespace m1;




/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}

Mesh* Tema2::CreateGate(const std::string& name) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	
	glm::vec3 frontColor = glm::vec3(0.2f, 0.6f, 1.0f);  // culoare pentru partea din fata
	glm::vec3 backColor = glm::vec3(1.0f, 0.4f, 0.4f);   // culoare pentru partea din spate

	float gateHeight = 15.0f;        // Height of the gate
	float gateWidth = 10.0f;         // Width of the gate
	float pillarThickness = 0.8f;    // Thickness of the pillars
	float barThickness = 0.9f;       // Thickness of the bars
	float gateDepth = 0.7f;          // Depth of the gate
	float groundLevel = -3.0f;       // Ground level for the bottom of the pillars

	// Create first vertical pillar
	std::vector<VertexFormat> pillarVertices = {
		// Front face
		VertexFormat(glm::vec3(-gateWidth / 2, groundLevel, -gateDepth), frontColor),
		VertexFormat(glm::vec3(-gateWidth / 2 + pillarThickness, groundLevel, -gateDepth), frontColor),
		VertexFormat(glm::vec3(-gateWidth / 2 + pillarThickness, gateHeight, -gateDepth), frontColor),
		VertexFormat(glm::vec3(-gateWidth / 2, gateHeight, -gateDepth), frontColor),

		// Back face
		VertexFormat(glm::vec3(-gateWidth / 2, groundLevel, gateDepth), backColor),
		VertexFormat(glm::vec3(-gateWidth / 2 + pillarThickness, groundLevel, gateDepth), backColor),
		VertexFormat(glm::vec3(-gateWidth / 2 + pillarThickness, gateHeight, gateDepth), backColor),
		VertexFormat(glm::vec3(-gateWidth / 2, gateHeight, gateDepth), backColor),
	};

	std::vector<unsigned int> pillarIndices = {
		0, 1, 2, 0, 2, 3,  // Front face
		4, 5, 6, 4, 6, 7,  // Back face
		0, 4, 7, 0, 7, 3,  // Left face
		1, 5, 6, 1, 6, 2,  // Right face
		3, 2, 6, 3, 6, 7,  // Top face
		4, 5, 1, 4, 1, 0   // Bottom face
	};

	vertices.insert(vertices.end(), pillarVertices.begin(), pillarVertices.end());
	indices.insert(indices.end(), pillarIndices.begin(), pillarIndices.end());

	// Create second vertical pillar (shifted to the right)
	for (auto& vertex : pillarVertices) {
		vertex.position.x += gateWidth - pillarThickness;  // Move to the right edge
	}
	for (auto index : pillarIndices) {
		indices.push_back(index + vertices.size());
	}
	vertices.insert(vertices.end(), pillarVertices.begin(), pillarVertices.end());

	// Function to create a horizontal bar with different colors on each side
	auto createHorizontalBar = [&](float heightPosition) {
		std::vector<VertexFormat> barVertices = {
			// Front face
			VertexFormat(glm::vec3(-gateWidth / 2, heightPosition, -gateDepth), frontColor),
			VertexFormat(glm::vec3(gateWidth / 2, heightPosition, -gateDepth), frontColor),
			VertexFormat(glm::vec3(gateWidth / 2, heightPosition + barThickness, -gateDepth), frontColor),
			VertexFormat(glm::vec3(-gateWidth / 2, heightPosition + barThickness, -gateDepth), frontColor),

			// Back face
			VertexFormat(glm::vec3(-gateWidth / 2, heightPosition, gateDepth), backColor),
			VertexFormat(glm::vec3(gateWidth / 2, heightPosition, gateDepth), backColor),
			VertexFormat(glm::vec3(gateWidth / 2, heightPosition + barThickness, gateDepth), backColor),
			VertexFormat(glm::vec3(-gateWidth / 2, heightPosition + barThickness, gateDepth), backColor),
		};

		std::vector<unsigned int> barIndices = {
			0, 1, 2, 0, 2, 3,  // Front face
			4, 5, 6, 4, 6, 7,  // Back face
			0, 4, 7, 0, 7, 3,  // Left face
			1, 5, 6, 1, 6, 2,  // Right face
			3, 2, 6, 3, 6, 7,  // Top face
			4, 5, 1, 4, 1, 0   // Bottom face
		};

		// Adjust indices for the current bar
		for (auto& index : barIndices) {
			index += vertices.size();
		}

		// Add bar vertices and indices
		vertices.insert(vertices.end(), barVertices.begin(), barVertices.end());
		indices.insert(indices.end(), barIndices.begin(), barIndices.end());
		};

	// Create middle horizontal bar
	createHorizontalBar(gateHeight * 0.5f);  // Positioned at half height

	// Create top horizontal bar
	createHorizontalBar(gateHeight * 0.95f);  // Positioned near the top

	Mesh* gate = new Mesh(name);
	gate->InitFromData(vertices, indices);
	return gate;
}
Mesh* Tema2::CreateSun(const std::string& name) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	
	glm::vec3 sunColor = glm::vec3(1.0f, 0.9f, 0.0f);     
	glm::vec3 rayColor = glm::vec3(1.0f, 0.7f, 0.0f);     
	float radius = 5.0f;     
	int segments = 20;       
	int numRays = 16;        
	float rayLength = 8.0f;   
	float rayWidth = 0.5f;    

	
	for (int i = 0; i <= segments; i++) {
		float phi = M_PI * float(i) / float(segments);
		for (int j = 0; j <= segments; j++) {
			float theta = 2.0f * M_PI * float(j) / float(segments);

			float x = radius * sin(phi) * cos(theta);
			float y = radius * cos(phi);
			float z = radius * sin(phi) * sin(theta);

			vertices.push_back(VertexFormat(glm::vec3(x, y, z), sunColor));
		}
	}

	
	for (int i = 0; i < segments; i++) {
		for (int j = 0; j < segments; j++) {
			int first = i * (segments + 1) + j;
			int second = first + segments + 1;

			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);

			indices.push_back(second);
			indices.push_back(second + 1);
			indices.push_back(first + 1);
		}
	}


	int baseIndex = vertices.size();
	for (int i = 0; i < numRays; i++) {
		float angle = 2.0f * M_PI * i / numRays;
		float cos_a = cos(angle);
		float sin_a = sin(angle);

		
		glm::vec3 rayStart = glm::vec3(radius * cos_a, radius * sin_a, 0);
		
		glm::vec3 rayEnd = glm::vec3((radius + rayLength) * cos_a, (radius + rayLength) * sin_a, 0);
		
		glm::vec3 perpVector = glm::vec3(-sin_a, cos_a, 0) * rayWidth;

	
		vertices.push_back(VertexFormat(rayStart + perpVector, rayColor));
		vertices.push_back(VertexFormat(rayStart - perpVector, rayColor));
		vertices.push_back(VertexFormat(rayEnd + perpVector, rayColor));
		vertices.push_back(VertexFormat(rayEnd - perpVector, rayColor));


		int idx = baseIndex + i * 4;
		indices.push_back(idx);
		indices.push_back(idx + 1);
		indices.push_back(idx + 2);

		indices.push_back(idx + 1);
		indices.push_back(idx + 3);
		indices.push_back(idx + 2);
	}

	Mesh* sun = new Mesh(name);
	sun->InitFromData(vertices, indices);
	return sun;
}

// adaugare minimap
void Tema2::RenderMinimap()
{
	
	glm::ivec2 resolution = window->GetResolution();
	int minimapSize = static_cast<int>(resolution.x * MINIMAP_SIZE);

	
	glClear(GL_DEPTH_BUFFER_BIT);

	
	glViewport(
		10,                              
		10,                             
		minimapSize,                   
		minimapSize                    
	);

	
	minimapCamera->position = dronePosition + glm::vec3(0, 50.0f, 0);

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -2, 0));
		RenderMesh(meshes["terrain"], shaders["VertexColor"], modelMatrix);
	}

	
	for (size_t i = 0; i < obstaclePositions.size(); i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, obstaclePositions[i]);
		modelMatrix = glm::scale(modelMatrix, obstacleScales[i]);
		RenderMesh(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
	}

	
	for (const auto& position : treePositions) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, position);
		RenderMesh(meshes["treeTrunk"], shaders["VertexColor"], modelMatrix);
	}

	
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, dronePosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f)); 
		RenderMesh(meshes["sphere"], shaders["VertexColor"], modelMatrix);
	}


	glViewport(0, 0, resolution.x, resolution.y);
}





void Tema2::Init()
{


	Mesh* timeText = CreateText("timeText");
	AddMeshToList(timeText);

	bearTime = 0.0f;


	meshes["smallTerrain"] = CreateSmallTerrain("smallTerrain");
	// adaugare camera pentru mini-map
	minimapCamera = new implemented::Camera();
	minimapCamera->Set(
		glm::vec3(0, 50.0f, 0),     
		glm::vec3(0, -1.0f, 0),     
		glm::vec3(0, 0, -1)         
	);

	
	float mapSize = 100.0f;
	minimapProjection = glm::ortho(-mapSize, mapSize, -mapSize, mapSize, 0.1f, 200.0f);
	renderCameraTarget = false;


	camera = new implemented::Camera();
	camera->Set(glm::vec3(0, cameraHeight, cameraHeight),
		dronePosition,
		glm::vec3(0, 1, 0));


	dronePosition = glm::vec3(0, 0, 0);
	propellerRotation = 0;


	thrust = 0.0f;
	droneVelocity = glm::vec3(0);
	droneRotation = glm::vec3(0);
	oldDronePosition = dronePosition;

	{
		meshes["droneBody"] = CreateDroneBody("droneBody");
		meshes["dronePropeller"] = CreateDronePropeller("dronePropeller");
	}

	// adaug terenul
	meshes["terrain"] = CreateTerrain("terrain");
	projectionMatrix = glm::perspective(RADIANS(90), window->props.aspectRatio, 0.01f, 200.0f);

	// meshes["obstacle"] = CreateObstacle("obstacle");

	int numObstacles = 25;
	for (int i = 0; i < numObstacles; ++i) {
		bool validPosition = false;
		while (!validPosition) {
			float x = rand() % 180 - 90;
			float z = rand() % 180 - 90;


			float scaleX = 1.0f + static_cast<float>(rand()) / RAND_MAX * 2.0f; 
			float scaleY = 2.0f + static_cast<float>(rand()) / RAND_MAX * 2.0f;  
			float scaleZ = 1.0f + static_cast<float>(rand()) / RAND_MAX * 2.0f;  

			
			validPosition = true;
			for (const auto& pos : treePositions) {
				if (glm::distance(glm::vec2(x, z), glm::vec2(pos.x, pos.z)) < 6.0f) {
					validPosition = false;
					break;
				}
			}
			for (const auto& pos : obstaclePositions) {
				if (glm::distance(glm::vec2(x, z), glm::vec2(pos.x, pos.z)) < 7.0f) {
					validPosition = false;
					break;
				}
			}

			if (validPosition) {
				obstaclePositions.push_back(glm::vec3(x, -1.0f, z));
				obstacleScales.push_back(glm::vec3(scaleX, scaleY, scaleZ));
			}

		}
	}

	meshes["treeTrunk"] = CreateTreeTrunk("treeTrunk");
	meshes["treeTop"] = CreateTreeTop("treeTop");

	int numTrees = 50;
	for (int i = 0; i < numTrees; i++) {
		bool validPosition = false;
		while (!validPosition) {
			float x = rand() % 180 - 90;
			float z = rand() % 180 - 90;

			validPosition = true;
			for (const auto& pos : treePositions) {
				if (glm::distance(glm::vec2(x, z), glm::vec2(pos.x, pos.z)) < 7.0f) {
					validPosition = false;
					break;
				}
			}

			if (validPosition) {
				treePositions.push_back(glm::vec3(x, -2.0f, z));
			}
		}
	}

	// afisez ursii
	{
		meshes["bearBody"] = CreateBearBody("bearBody");
		meshes["bearHead"] = CreateBearHead("bearHead");
		meshes["bearEar"] = CreateBearEar("bearEar");
		meshes["bearLeg"] = CreateBearLeg("bearLeg");
	}
	

	int numBears = 10;
	for (int i = 0; i < numBears; i++) {
		bool validPosition = false;
		while (!validPosition) {
			float x = -90.0f + static_cast<float>(rand()) / RAND_MAX * 180.0f;
			float z = -90.0f + static_cast<float>(rand()) / RAND_MAX * 180.0f;

			validPosition = true;
			
			for (const auto& pos : treePositions) {
				if (glm::distance(glm::vec2(x, z), glm::vec2(pos.x, pos.z)) < 7.0f) {
					validPosition = false;
					break;
				}
			}
		
			for (const auto& pos : bearPositions) {
				if (glm::distance(glm::vec2(x, z), glm::vec2(pos.x, pos.z)) < 10.0f) {
					validPosition = false;
					break;
				}
			}

			if (validPosition) {
				bearPositions.push_back(glm::vec3(x, -2.0f, z));
				bearRotations.push_back(static_cast<float>(rand()) / RAND_MAX * 360.0f);  
			}
		}
	}
	int numGates = 10;  
	for (int i = 0; i < numGates; i++) {
		bool validPosition = false;

		while (!validPosition) {
			float x = -80.0f + static_cast<float>(rand()) / RAND_MAX * 160.0f;
			float z = -80.0f + static_cast<float>(rand()) / RAND_MAX * 160.0f;

			validPosition = true;

			
			for (const auto& pos : obstaclePositions) {
				if (glm::distance(glm::vec2(x, z), glm::vec2(pos.x, pos.z)) < 10.0f) {
					validPosition = false;
					break;
				}
			}

			if (validPosition) {
				gatePositions.push_back(glm::vec3(x, -1.0f, z));
			}
		}
	}

	
	meshes["gate"] = CreateGate("gate");

	// creare flori
	meshes["flower"] = CreateFlower("flower");



	
	int numFlowers = 100;  
	for (int i = 0; i < numFlowers; i++) {
		bool validPosition = false;
		while (!validPosition) {
			float x = -90.0f + static_cast<float>(rand()) / RAND_MAX * 180.0f;
			float z = -90.0f + static_cast<float>(rand()) / RAND_MAX * 180.0f;

			validPosition = true;

			
			for (const auto& pos : treePositions) {
				if (glm::distance(glm::vec2(x, z), glm::vec2(pos.x, pos.z)) < 3.0f) {
					validPosition = false;
					break;
				}
			}

			
			for (const auto& pos : flowerPositions) {
				if (glm::distance(glm::vec2(x, z), glm::vec2(pos.x, pos.z)) < 1.0f) {
					validPosition = false;
					break;
				}
			}

			if (validPosition) {
				flowerPositions.push_back(glm::vec3(x, -2.0f, z));
			}
		}
	}

	// afisare soare
	meshes["sun"] = CreateSun("sun");

}



Mesh* Tema2::CreateBearBody(const std::string& name) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;


	glm::vec3 mainColor = glm::vec3(0.4f, 0.25f, 0.1f);   
	glm::vec3 bellyColor = glm::vec3(0.5f, 0.35f, 0.2f);   


	float width = 1.7f;
	float height = 1.2f;
	float length = 3.0f;

	
	std::vector<VertexFormat> mainBodyVertices = {
		
		VertexFormat(glm::vec3(-width / 2, -height / 2, length / 2), mainColor),
		VertexFormat(glm::vec3(width / 2, -height / 2, length / 2), mainColor),
		VertexFormat(glm::vec3(width / 2, height / 2, length / 2), mainColor),
		VertexFormat(glm::vec3(-width / 2, height / 2, length / 2), mainColor),

	
		VertexFormat(glm::vec3(-width / 2, -height / 2, -length / 2), mainColor),
		VertexFormat(glm::vec3(width / 2, -height / 2, -length / 2), mainColor),
		VertexFormat(glm::vec3(width / 2, height / 2, -length / 2), mainColor),
		VertexFormat(glm::vec3(-width / 2, height / 2, -length / 2), mainColor),
	};

	
	float bellyWidth = width * 1.1f;
	std::vector<VertexFormat> bellyVertices = {
		VertexFormat(glm::vec3(-bellyWidth / 2, -height / 2, length / 2), bellyColor),
		VertexFormat(glm::vec3(bellyWidth / 2, -height / 2, length / 2), bellyColor),
		VertexFormat(glm::vec3(bellyWidth / 2, -height / 4, length / 2), bellyColor),
		VertexFormat(glm::vec3(-bellyWidth / 2, -height / 4, length / 2), bellyColor),

		VertexFormat(glm::vec3(-bellyWidth / 2, -height / 2, -length / 2), bellyColor),
		VertexFormat(glm::vec3(bellyWidth / 2, -height / 2, -length / 2), bellyColor),
		VertexFormat(glm::vec3(bellyWidth / 2, -height / 4, -length / 2), bellyColor),
		VertexFormat(glm::vec3(-bellyWidth / 2, -height / 4, -length / 2), bellyColor),
	};

	
	std::vector<unsigned int> bodyIndices = {
		0, 1, 2,    0, 2, 3,   
		1, 5, 6,    1, 6, 2,   
		5, 4, 7,    5, 7, 6,   
		4, 0, 3,    4, 3, 7,   
		3, 2, 6,    3, 6, 7,   
		4, 5, 1,    4, 1, 0    
	};

	vertices.insert(vertices.end(), mainBodyVertices.begin(), mainBodyVertices.end());
	indices.insert(indices.end(), bodyIndices.begin(), bodyIndices.end());


	int baseIndex = vertices.size();
	vertices.insert(vertices.end(), bellyVertices.begin(), bellyVertices.end());
	for (int i = 0; i < bodyIndices.size(); i++) {
		indices.push_back(bodyIndices[i] + baseIndex);
	}

	Mesh* body = new Mesh(name);
	body->InitFromData(vertices, indices);
	return body;
}

Mesh* Tema2::CreateBearEar(const std::string& name) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	glm::vec3 earColor = glm::vec3(0.35f, 0.2f, 0.1f);  
	float radius = 0.2f;     
	float height = 0.3f;     
	int segments = 12;       

	
	vertices.push_back(VertexFormat(glm::vec3(0, height, 0), earColor));


	for (int i = 0; i < segments; i++) {
		float angle = 2.0f * M_PI * i / segments;
		vertices.push_back(VertexFormat(
			glm::vec3(radius * cos(angle), 0, radius * sin(angle)),
			earColor
		));
	}

	
	for (int i = 1; i < segments; i++) {
		
		indices.push_back(0);         
		indices.push_back(i);        
		indices.push_back(i + 1);    
	}

	
	indices.push_back(0);
	indices.push_back(segments);
	indices.push_back(1);


	for (int i = 1; i < segments - 1; i++) {
		indices.push_back(1);
		indices.push_back(i + 1);
		indices.push_back(i + 2);
	}

	Mesh* ear = new Mesh(name);
	ear->InitFromData(vertices, indices);
	return ear;
}

Mesh* Tema2::CreateBearHead(const std::string& name) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	glm::vec3 mainColor = glm::vec3(0.4f, 0.25f, 0.1f);    
	glm::vec3 snoutColor = glm::vec3(0.3f, 0.15f, 0.05f);  


	float width = 1.5f;
	float height = 1.35f;
	float length = 1.2f;

	std::vector<VertexFormat> headVertices = {
	
		VertexFormat(glm::vec3(-width / 2, -height / 2, length / 2), mainColor),
		VertexFormat(glm::vec3(width / 2, -height / 2, length / 2), mainColor),
		VertexFormat(glm::vec3(width / 2, height / 2, length / 2), mainColor),
		VertexFormat(glm::vec3(-width / 2, height / 2, length / 2), mainColor),

	
		VertexFormat(glm::vec3(-width / 2, -height / 2, -length / 2), mainColor),
		VertexFormat(glm::vec3(width / 2, -height / 2, -length / 2), mainColor),
		VertexFormat(glm::vec3(width / 2, height / 2, -length / 2), mainColor),
		VertexFormat(glm::vec3(-width / 2, height / 2, -length / 2), mainColor),
	};


	float snoutSize = width * 0.4f;
	float snoutProtrusion = length * 0.3f;
	std::vector<VertexFormat> snoutVertices = {
		VertexFormat(glm::vec3(-snoutSize / 2, -snoutSize / 2, length / 2 + snoutProtrusion), snoutColor),
		VertexFormat(glm::vec3(snoutSize / 2, -snoutSize / 2, length / 2 + snoutProtrusion), snoutColor),
		VertexFormat(glm::vec3(snoutSize / 2, snoutSize / 2, length / 2 + snoutProtrusion), snoutColor),
		VertexFormat(glm::vec3(-snoutSize / 2, snoutSize / 2, length / 2 + snoutProtrusion), snoutColor),

		VertexFormat(glm::vec3(-snoutSize / 2, -snoutSize / 2, length / 2), snoutColor),
		VertexFormat(glm::vec3(snoutSize / 2, -snoutSize / 2, length / 2), snoutColor),
		VertexFormat(glm::vec3(snoutSize / 2, snoutSize / 2, length / 2), snoutColor),
		VertexFormat(glm::vec3(-snoutSize / 2, snoutSize / 2, length / 2), snoutColor),
	};

	std::vector<unsigned int> cubeIndices = {
		0, 1, 2,    0, 2, 3,   
		1, 5, 6,    1, 6, 2,   
		5, 4, 7,    5, 7, 6,   
		4, 0, 3,    4, 3, 7,   
		3, 2, 6,    3, 6, 7,   
		4, 5, 1,    4, 1, 0    
	};
	

	vertices.insert(vertices.end(), headVertices.begin(), headVertices.end());
	indices.insert(indices.end(), cubeIndices.begin(), cubeIndices.end());


	int baseIndex = vertices.size();
	vertices.insert(vertices.end(), snoutVertices.begin(), snoutVertices.end());
	for (int i = 0; i < cubeIndices.size(); i++) {
		indices.push_back(cubeIndices[i] + baseIndex);
	}

	Mesh* head = new Mesh(name);
	head->InitFromData(vertices, indices);
	return head;
}

Mesh* Tema2::CreateBearLeg(const std::string& name) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	glm::vec3 legColor = glm::vec3(0.35f, 0.2f, 0.1f);
	glm::vec3 pawColor = glm::vec3(0.3f, 0.15f, 0.05f);

	
	float width = 1.0f;
	float height = 2.0f;
	float depth = 1.0f;

	std::vector<VertexFormat> legVertices = {
		
		VertexFormat(glm::vec3(-width / 2, -height / 2, depth / 2), legColor),
		VertexFormat(glm::vec3(width / 2, -height / 2, depth / 2), legColor),
		VertexFormat(glm::vec3(width / 2, height / 2, depth / 2), legColor),
		VertexFormat(glm::vec3(-width / 2, height / 2, depth / 2), legColor),

		
		VertexFormat(glm::vec3(-width / 2, -height / 2, -depth / 2), legColor),
		VertexFormat(glm::vec3(width / 2, -height / 2, -depth / 2), legColor),
		VertexFormat(glm::vec3(width / 2, height / 2, -depth / 2), legColor),
		VertexFormat(glm::vec3(-width / 2, height / 2, -depth / 2), legColor),
	};

	float pawWidth = width * 1.3f;
	float pawHeight = height * 0.2f;
	std::vector<VertexFormat> pawVertices = {
		VertexFormat(glm::vec3(-pawWidth / 2, -height / 2 - pawHeight, depth / 2), pawColor),
		VertexFormat(glm::vec3(pawWidth / 2, -height / 2 - pawHeight, depth / 2), pawColor),
		VertexFormat(glm::vec3(pawWidth / 2, -height / 2, depth / 2), pawColor),
		VertexFormat(glm::vec3(-pawWidth / 2, -height / 2, depth / 2), pawColor),

		VertexFormat(glm::vec3(-pawWidth / 2, -height / 2 - pawHeight, -depth / 2), pawColor),
		VertexFormat(glm::vec3(pawWidth / 2, -height / 2 - pawHeight, -depth / 2), pawColor),
		VertexFormat(glm::vec3(pawWidth / 2, -height / 2, -depth / 2), pawColor),
		VertexFormat(glm::vec3(-pawWidth / 2, -height / 2, -depth / 2), pawColor),
	};

	std::vector<unsigned int> cubeIndices = {
		0, 1, 2,    0, 2, 3,   
		1, 5, 6,    1, 6, 2,   
		5, 4, 7,    5, 7, 6,   
		4, 0, 3,    4, 3, 7,   
		3, 2, 6,    3, 6, 7,   
		4, 5, 1,    4, 1, 0    
	};

	
	vertices.insert(vertices.end(), legVertices.begin(), legVertices.end());
	indices.insert(indices.end(), cubeIndices.begin(), cubeIndices.end());


	int baseIndex = vertices.size();
	vertices.insert(vertices.end(), pawVertices.begin(), pawVertices.end());
	for (int i = 0; i < cubeIndices.size(); i++) {
		indices.push_back(cubeIndices[i] + baseIndex);
	}

	Mesh* leg = new Mesh(name);
	leg->InitFromData(vertices, indices);
	return leg;
}

Mesh* Tema2::CreateTreeTop(const std::string& name) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	
	glm::vec3 darkGreen = glm::vec3(0.0f, 0.25f, 0.0f);
	glm::vec3 lightGreen = glm::vec3(0.1f, 0.35f, 0.1f);

	float baseRadius = 1.5f;    
	float totalHeight = 4.0f;   
	int numLevels = 6;          


	float levelHeight = totalHeight / (numLevels - 1);

	
	for (int level = 0; level < numLevels; level++) {
		float y = level * levelHeight;
		
		float currentRadius = baseRadius * (1.0f - (float)level / numLevels);

		
		float t = (float)level / (numLevels - 1);
		glm::vec3 currentColor = glm::mix(darkGreen, lightGreen, t);

		
		int numTriangles = 8;
		float angleStep = 2.0f * M_PI / numTriangles;

		for (int i = 0; i < numTriangles; i++) {
			float angle1 = i * angleStep;
			float angle2 = (i + 1) * angleStep;

		
			vertices.push_back(VertexFormat(
				glm::vec3(0, y + levelHeight * 0.7f, 0),
				currentColor
			));

			
			vertices.push_back(VertexFormat(
				glm::vec3(currentRadius * cos(angle1), y, currentRadius * sin(angle1)),
				currentColor
			));

			vertices.push_back(VertexFormat(
				glm::vec3(currentRadius * cos(angle2), y, currentRadius * sin(angle2)),
				currentColor
			));

			
			int baseIndex = vertices.size() - 3;
			indices.push_back(baseIndex);     
			indices.push_back(baseIndex + 1); 
			indices.push_back(baseIndex + 2); 

			
			if (level > 0) {
				float lowerRadius = baseRadius * (1.0f - (float)(level - 1) / numLevels);

				vertices.push_back(VertexFormat(
					glm::vec3(currentRadius * cos(angle1), y, currentRadius * sin(angle1)),
					currentColor
				));

				vertices.push_back(VertexFormat(
					glm::vec3(currentRadius * cos(angle2), y, currentRadius * sin(angle2)),
					currentColor
				));

				vertices.push_back(VertexFormat(
					glm::vec3(lowerRadius * cos((angle1 + angle2) / 2), y - levelHeight,
						lowerRadius * sin((angle1 + angle2) / 2)),
					currentColor
				));

				baseIndex = vertices.size() - 3;
				indices.push_back(baseIndex);
				indices.push_back(baseIndex + 1);
				indices.push_back(baseIndex + 2);
			}
		}
	}

	
	vertices.push_back(VertexFormat(
		glm::vec3(0, totalHeight, 0),
		lightGreen
	));

	for (int i = 0; i < 8; i++) {
		float angle = i * 2.0f * M_PI / 8;
		float smallRadius = baseRadius * 0.1f;

		vertices.push_back(VertexFormat(
			glm::vec3(smallRadius * cos(angle), totalHeight - levelHeight, smallRadius * sin(angle)),
			lightGreen
		));

		if (i < 7) {
			indices.push_back(vertices.size() - 9); 
			indices.push_back(vertices.size() - 1); 
			indices.push_back(vertices.size());      
		}
		else {
			indices.push_back(vertices.size() - 9);  
			indices.push_back(vertices.size() - 1);  
			indices.push_back(vertices.size() - 8);  
		}
	}

	Mesh* treeTop = new Mesh(name);
	treeTop->InitFromData(vertices, indices);
	return treeTop;
}

// creare trunchi teren
Mesh* Tema2::CreateTreeTrunk(const std::string& name) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	
	glm::vec3 darkBrown = glm::vec3(0.3f, 0.2f, 0.1f);
	glm::vec3 lightBrown = glm::vec3(0.4f, 0.25f, 0.15f);

	float height = 3.5f;    
	float baseRadius = 0.4f; 
	float topRadius = 0.3f;  
	int segments = 8;       

	
	for (int i = 0; i <= segments; i++) {
		float angle = 2.0f * M_PI * i / segments;
		float c = cos(angle);
		float s = sin(angle);

	
		vertices.push_back(VertexFormat(
			glm::vec3(baseRadius * c, 0, baseRadius * s),
			darkBrown
		));

		
		vertices.push_back(VertexFormat(
			glm::vec3(topRadius * c, height, topRadius * s),
			lightBrown
		));
	}


	for (int i = 0; i < segments; i++) {
		int base0 = i * 2;
		int base1 = (i * 2 + 2) % (segments * 2 + 2);
		int top0 = i * 2 + 1;
		int top1 = (i * 2 + 3) % (segments * 2 + 2);

		indices.push_back(base0);
		indices.push_back(base1);
		indices.push_back(top0);

		indices.push_back(base1);
		indices.push_back(top1);
		indices.push_back(top0);
	}

	Mesh* trunk = new Mesh(name);
	trunk->InitFromData(vertices, indices);
	return trunk;
}


// creare teren
Mesh* Tema2::CreateTerrain(const std::string& name)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	float size = 100.0f;
	float y = -1.0f;


	glm::vec3 greenColor(0.2f, 0.45f, 0.1f);
	glm::vec3 brownColor(0.36f, 0.25f, 0.20f);
	int gridSize = 40;
	float cellSize = size * 2 / gridSize;


	for (int i = 0; i <= gridSize; i++) {
		for (int j = 0; j <= gridSize; j++) {
			float x = -size + j * cellSize;
			float z = -size + i * cellSize;


			float randomValue = sin(x * 0.15f + z * 0.15f) * sin(x * 0.2f - z * 0.15f);
			randomValue = (randomValue + 1.0f) * 0.5f;

			glm::vec3 color;

			if (randomValue > 0.6f) {
				color = brownColor;
			}
			else {
				color = greenColor;
			}

			vertices.push_back(VertexFormat(glm::vec3(x, y, z), color));
		}
	}


	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			int topLeft = i * (gridSize + 1) + j;
			int topRight = topLeft + 1;
			int bottomLeft = (i + 1) * (gridSize + 1) + j;
			int bottomRight = bottomLeft + 1;

			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}

	Mesh* terrain = new Mesh(name);
	terrain->InitFromData(vertices, indices);
	terrain->SetDrawMode(GL_TRIANGLES);
	return terrain;
}

// construire corp drona
Mesh* Tema2::CreateDroneBody(const std::string& name)
{
	glm::vec3 gray = glm::vec3(0.7f);

	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;


	float length = 0.5f;
	float width = 0.05f;
	float height = 0.05f;


	vertices = {

		VertexFormat(glm::vec3(-length, -height, -length), gray),
		VertexFormat(glm::vec3(length, -height, length), gray),
		VertexFormat(glm::vec3(length + width, -height, length - width), gray),
		VertexFormat(glm::vec3(-length + width, -height, -length - width), gray),


		VertexFormat(glm::vec3(-length, height, -length), gray),
		VertexFormat(glm::vec3(length, height, length), gray),
		VertexFormat(glm::vec3(length + width, height, length - width), gray),
		VertexFormat(glm::vec3(-length + width, height, -length - width), gray),
	};

	vertices.push_back(VertexFormat(glm::vec3(-length, -height, length), gray));
	vertices.push_back(VertexFormat(glm::vec3(length, -height, -length), gray));
	vertices.push_back(VertexFormat(glm::vec3(length + width, -height, -length + width), gray));
	vertices.push_back(VertexFormat(glm::vec3(-length + width, -height, length + width), gray));

	vertices.push_back(VertexFormat(glm::vec3(-length, height, length), gray));
	vertices.push_back(VertexFormat(glm::vec3(length, height, -length), gray));
	vertices.push_back(VertexFormat(glm::vec3(length + width, height, -length + width), gray));
	vertices.push_back(VertexFormat(glm::vec3(-length + width, height, length + width), gray));


	indices = {
		0, 1, 2,    0, 2, 3,
		4, 5, 6,    4, 6, 7,
		0, 4, 7,    0, 7, 3,
		1, 5, 6,    1, 6, 2,
		3, 2, 6,    3, 6, 7,
		0, 1, 5,    0, 5, 4,
	};


	for (int i = 0; i < 36; i += 3) {
		indices.push_back(indices[i] + 8);
		indices.push_back(indices[i + 1] + 8);
		indices.push_back(indices[i + 2] + 8);
	}

	Mesh* mesh = new Mesh(name);
	mesh->InitFromData(vertices, indices);
	return mesh;
}

Mesh* Tema2::CreateDronePropeller(const std::string& name)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;


	glm::vec3 black = glm::vec3(0, 0, 0);


	float length = 0.4f;
	float width = 0.05f;
	float height = 0.02f;

	vertices = {

		VertexFormat(glm::vec3(-length, -height, -width), black),
		VertexFormat(glm::vec3(length, -height, -width), black),
		VertexFormat(glm::vec3(length, -height, width), black),
		VertexFormat(glm::vec3(-length, -height, width), black),


		VertexFormat(glm::vec3(-length, height, -width), black),
		VertexFormat(glm::vec3(length, height, -width), black),
		VertexFormat(glm::vec3(length, height, width), black),
		VertexFormat(glm::vec3(-length, height, width), black),
	};

	indices = {
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		0, 4, 7,
		0, 7, 3,
		1, 5, 6,
		1, 6, 2,
		0, 1, 5,
		0, 5, 4,
		3, 2, 6,
		3, 6, 7,
	};

	Mesh* propeller = new Mesh(name);
	propeller->InitFromData(vertices, indices);
	return propeller;
}

void Tema2::RenderDrone(float deltaTimeSeconds)
{

	propellerRotation += deltaTimeSeconds * 10.0f;


	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, dronePosition);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
	RenderMesh(meshes["droneBody"], shaders["VertexColor"], modelMatrix);


	glm::vec3 propellerPositions[] = {
		glm::vec3(0.3f, 0.1f, 0.3f),
		glm::vec3(-0.3f, 0.1f, 0.3f),
		glm::vec3(0.3f, 0.1f, -0.3f),
		glm::vec3(-0.3f, 0.1f, -0.3f),
	};


	for (const auto& pos : propellerPositions) {
		glm::mat4 propellerMatrix = glm::mat4(1);
		propellerMatrix = glm::translate(modelMatrix, pos);
		propellerMatrix = glm::rotate(propellerMatrix, propellerRotation, glm::vec3(0, 1, 0));
		RenderMesh(meshes["dronePropeller"], shaders["VertexColor"], propellerMatrix);
	}
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

// functie pentru crearea florilor
Mesh* Tema2::CreateFlower(const std::string& name) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	
	glm::vec3 petalColor = glm::vec3(1.0f, 0.75f, 0.8f);    
	glm::vec3 centerColor = glm::vec3(1.0f, 1.0f, 0.0f);    
	glm::vec3 stemColor = glm::vec3(0.0f, 0.5f, 0.0f);      

	float petalSize = 0.4f;
	float centerSize = 0.3f;
	float stemHeight = 1.0f;
	float stemWidth = 0.08f;

	
	vertices.push_back(VertexFormat(glm::vec3(-stemWidth, 0, -stemWidth), stemColor));
	vertices.push_back(VertexFormat(glm::vec3(stemWidth, 0, -stemWidth), stemColor));
	vertices.push_back(VertexFormat(glm::vec3(stemWidth, stemHeight, -stemWidth), stemColor));
	vertices.push_back(VertexFormat(glm::vec3(-stemWidth, stemHeight, -stemWidth), stemColor));
	vertices.push_back(VertexFormat(glm::vec3(-stemWidth, 0, stemWidth), stemColor));
	vertices.push_back(VertexFormat(glm::vec3(stemWidth, 0, stemWidth), stemColor));
	vertices.push_back(VertexFormat(glm::vec3(stemWidth, stemHeight, stemWidth), stemColor));
	vertices.push_back(VertexFormat(glm::vec3(-stemWidth, stemHeight, stemWidth), stemColor));

	
	std::vector<unsigned int> stemIndices = {
		0, 1, 2,    0, 2, 3,  
		1, 5, 6,    1, 6, 2,  
		5, 4, 7,    5, 7, 6,  
		4, 0, 3,    4, 3, 7,  
		3, 2, 6,    3, 6, 7,  
		4, 5, 1,    4, 1, 0   
	};
	indices.insert(indices.end(), stemIndices.begin(), stemIndices.end());


	int numPetals = 5;
	float angleStep = 2 * M_PI / numPetals;
	float baseHeight = stemHeight;

	for (int i = 0; i < numPetals; i++) {
		float angle = i * angleStep;
		float nextAngle = (i + 1) * angleStep;

		
		glm::vec3 tip(petalSize * cos(angle + angleStep / 2),
			baseHeight + 0.1f,
			petalSize * sin(angle + angleStep / 2));

		
		glm::vec3 base1(0.4f * petalSize * cos(angle), baseHeight,
			0.4f * petalSize * sin(angle));
		glm::vec3 base2(0.4f * petalSize * cos(nextAngle), baseHeight,
			0.4f * petalSize * sin(nextAngle));

		
		vertices.push_back(VertexFormat(base1, petalColor));
		vertices.push_back(VertexFormat(base2, petalColor));
		vertices.push_back(VertexFormat(tip, petalColor));

		
		int baseIndex = vertices.size() - 3;
		indices.push_back(baseIndex);
		indices.push_back(baseIndex + 1);
		indices.push_back(baseIndex + 2);
	}

	
	int centerSegments = 16;
	float centerHeight = stemHeight + 0.05f;
	vertices.push_back(VertexFormat(glm::vec3(0, centerHeight, 0), centerColor)); 

	for (int i = 0; i < centerSegments; i++) {
		float angle = i * 2 * M_PI / centerSegments;
		vertices.push_back(VertexFormat(
			glm::vec3(centerSize * cos(angle), centerHeight, centerSize * sin(angle)),
			centerColor
		));
	}


	for (int i = 0; i < centerSegments; i++) {
		indices.push_back(vertices.size() - centerSegments - 1); 
		indices.push_back(vertices.size() - centerSegments + i);
		indices.push_back(vertices.size() - centerSegments + (i + 1) % centerSegments);
	}

	Mesh* flower = new Mesh(name);
	flower->InitFromData(vertices, indices);
	return flower;
}


void Tema2::DrawRectangle(float x, float y, float width, float height, glm::vec3 color, float alpha) {
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	glColor4f(color.r, color.g, color.b, alpha);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Tema2::RenderText(const std::string& text, float x, float y, glm::vec3 color)
{
	glDisable(GL_DEPTH_TEST);

	float scale = 0.5f;
	float offset = 0;

	for (char c : text) {
		float charWidth = 20.0f * scale;
		float charHeight = 30.0f * scale;

		glBegin(GL_QUADS);
		glColor3f(color.r, color.g, color.b);
		glVertex2f(x + offset, y);
		glVertex2f(x + offset + charWidth, y);
		glVertex2f(x + offset + charWidth, y + charHeight);
		glVertex2f(x + offset, y + charHeight);
		glEnd();

		offset += charWidth + 2.0f;
	}

	glEnable(GL_DEPTH_TEST);
}

void Tema2::Update(float deltaTimeSeconds)
{

	if (!timerExpired && raceStarted) {
		gameTimer -= deltaTimeSeconds;
		if (gameTimer <= 0) {
			gameTimer = 0;
			timerExpired = true;
			raceFinished = true;
		}
	}

	glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// glEnable(GL_DEPTH_TEST);

	RenderDrone(deltaTimeSeconds);

	if (renderCameraTarget)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderMesh(meshes["sphere"], shaders["VertexColor"], modelMatrix);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -2, 0));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		RenderMesh(meshes["terrain"], shaders["VertexColor"], modelMatrix);
	}


	for (size_t i = 0; i < obstaclePositions.size(); i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, obstaclePositions[i]);
		modelMatrix = glm::scale(modelMatrix, obstacleScales[i]);
		RenderMesh(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
	}


	for (const auto& position : treePositions) {
		glm::mat4 trunkMatrix = glm::mat4(1);
		trunkMatrix = glm::translate(trunkMatrix, position);
		RenderMesh(meshes["treeTrunk"], shaders["VertexColor"], trunkMatrix);

		glm::mat4 topMatrix = glm::mat4(1);
		topMatrix = glm::translate(topMatrix, position + glm::vec3(0, 3.0f, 0));
		RenderMesh(meshes["treeTop"], shaders["VertexColor"], topMatrix);
	}

	bearTime += deltaTimeSeconds;


	for (size_t i = 0; i < bearPositions.size(); i++) {
		float bodyBob = sin(bearTime * 2.0f + i) * 0.1f;
		float headTilt = sin(bearTime * 1.5f + i) * 10.0f;
		float legSwing = sin(bearTime * 3.0f + i) * 15.0f;
		float earWiggle = sin(bearTime * 4.0f + i) * 5.0f;

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, bearPositions[i] + glm::vec3(0, bodyBob, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(bearRotations[i]), glm::vec3(0, 1, 0));

		RenderMesh(meshes["bearBody"], shaders["VertexColor"], modelMatrix);

		glm::mat4 headMatrix = modelMatrix;
		headMatrix = glm::translate(headMatrix, glm::vec3(0, 0.9f, 1.8f));
		headMatrix = glm::rotate(headMatrix, RADIANS(-15.0f + headTilt), glm::vec3(1, 0, 0));
		RenderMesh(meshes["bearHead"], shaders["VertexColor"], headMatrix);

		glm::mat4 leftEarMatrix = headMatrix;
		leftEarMatrix = glm::translate(leftEarMatrix, glm::vec3(-0.5f, 0.8f, 0));
		leftEarMatrix = glm::rotate(leftEarMatrix, RADIANS(-30.0f + earWiggle), glm::vec3(0, 0, 1));
		RenderMesh(meshes["bearEar"], shaders["VertexColor"], leftEarMatrix);

		glm::mat4 rightEarMatrix = headMatrix;
		rightEarMatrix = glm::translate(rightEarMatrix, glm::vec3(0.5f, 0.8f, 0));
		rightEarMatrix = glm::rotate(rightEarMatrix, RADIANS(30.0f - earWiggle), glm::vec3(0, 0, 1));
		RenderMesh(meshes["bearEar"], shaders["VertexColor"], rightEarMatrix);

		std::vector<glm::vec3> legPositions = {
			glm::vec3(0.8f, -1.5f, 1.2f),
			glm::vec3(-0.8f, -1.5f, 1.2f),
			glm::vec3(0.8f, -1.5f, -1.2f),
			glm::vec3(-0.8f, -1.5f, -1.2f)
		};

		for (size_t j = 0; j < legPositions.size(); j++) {
			glm::mat4 legMatrix = modelMatrix;
			legMatrix = glm::translate(legMatrix, legPositions[j]);
			float legPhase = bearTime * 3.0f + (j * M_PI / 2);
			float legOffset = sin(legPhase) * 0.2f;
			legMatrix = glm::translate(legMatrix, glm::vec3(0, legOffset, 0));
			float legRotation = cos(legPhase) * legSwing;
			legMatrix = glm::rotate(legMatrix, RADIANS(legRotation), glm::vec3(1, 0, 0));
			RenderMesh(meshes["bearLeg"], shaders["VertexColor"], legMatrix);
		}
	}


	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(80.0f, -2.0f, 80.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
	RenderMesh(meshes["smallTerrain"], shaders["VertexColor"], modelMatrix);

	RenderMinimap();


	for (size_t i = 0; i < gatePositions.size(); i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, gatePositions[i]);
		RenderMesh(meshes["gate"], shaders["VertexColor"], modelMatrix);
	}


	for (const auto& position : flowerPositions) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(static_cast<float>(rand() % 360)), glm::vec3(0, 1, 0));
		RenderMesh(meshes["flower"], shaders["VertexColor"], modelMatrix);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-40.0f, 30.0f, -40.0f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, (float)Engine::GetElapsedTime(), glm::vec3(0, 0, 1));
		RenderMesh(meshes["sun"], shaders["VertexColor"], modelMatrix);
	}


	


	// afisare timer
	glm::ivec2 resolution = window->GetResolution();
	int minutes = (int)gameTimer / 60;
	int seconds = (int)gameTimer % 60;
	std::string timeText = std::to_string(minutes) + ":" +
		(seconds < 10 ? "0" : "") + std::to_string(seconds);

	
	int timerX = resolution.x - 150;
	int timerY = resolution.y - 50;

	
	DrawRectangle(timerX - 10, timerY - 30, 140, 40, glm::vec3(0, 0, 0), 0.5f);

	
	if (timerExpired) {
		RenderText("Time's UP!", timerX, timerY, glm::vec3(1, 0, 0));  
	}
	else {
		RenderText("Time: " + timeText, timerX, timerY, glm::vec3(1, 1, 1));  
	}
}


Mesh* Tema2::CreateText(const std::string& name)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	
	float charWidth = 20.0f;
	float charHeight = 30.0f;

	vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1)));
	vertices.push_back(VertexFormat(glm::vec3(charWidth, 0, 0), glm::vec3(1)));
	vertices.push_back(VertexFormat(glm::vec3(charWidth, charHeight, 0), glm::vec3(1)));
	vertices.push_back(VertexFormat(glm::vec3(0, charHeight, 0), glm::vec3(1)));

	indices = { 0, 1, 2, 0, 2, 3 };

	Mesh* text = new Mesh(name);
	text->InitFromData(vertices, indices);
	return text;
}

void Tema2::FrameEnd()
{
	// DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

Mesh* Tema2::CreateSmallTerrain(const std::string& name)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	float size = 20.0f;
	float y = -1.0f;

	glm::vec3 greenColor(0.2f, 0.45f, 0.1f);

	int gridSize = 10;
	float cellSize = size * 2 / gridSize;

	for (int i = 0; i <= gridSize; i++) {
		for (int j = 0; j <= gridSize; j++) {
			float x = -size + j * cellSize;
			float z = -size + i * cellSize;

			vertices.push_back(VertexFormat(glm::vec3(x, y, z), greenColor));
		}
	}

	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			int topLeft = i * (gridSize + 1) + j;
			int topRight = topLeft + 1;
			int bottomLeft = (i + 1) * (gridSize + 1) + j;
			int bottomRight = bottomLeft + 1;

			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}

	Mesh* smallTerrain = new Mesh(name);
	smallTerrain->InitFromData(vertices, indices);
	smallTerrain->SetDrawMode(GL_TRIANGLES);
	return smallTerrain;
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


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */





// functii care lasa sau nu drona sa se miste

bool Tema2::SphereAABBCollision(const glm::vec3& sphereCenter, float sphereRadius,
	const glm::vec3& boxMin, const glm::vec3& boxMax)
{
	
	float x = std::max(boxMin.x, std::min(sphereCenter.x, boxMax.x));
	float y = std::max(boxMin.y, std::min(sphereCenter.y, boxMax.y));
	float z = std::max(boxMin.z, std::min(sphereCenter.z, boxMax.z));

	float distance = glm::length(glm::vec3(x, y, z) - sphereCenter);

	return distance < sphereRadius;
}

bool Tema2::CheckCollisionWithGround()
{
	
	return dronePosition.y - droneRadius < -2.0f;
}



bool Tema2::CheckCollisionWithTrees()
{
	for (const auto& treePos : treePositions) {
		
		float dx = dronePosition.x - treePos.x;
		float dz = dronePosition.z - treePos.z;
		float distanceHorizontal = sqrt(dx * dx + dz * dz);

	
		if (distanceHorizontal < (DRONE_RADIUS + TREE_TRUNK_RADIUS)) {
		
			if (dronePosition.y >= treePos.y && dronePosition.y <= treePos.y + TREE_HEIGHT) {
				return true; 
			}
		}
	}
	return false;
}

bool Tema2::CheckGateCollision(const glm::vec3& sphereCenter, float sphereRadius, const glm::vec3& gatePosition) {
	float gateWidth = 4.0f;
	float pillarThickness = 0.4f;
	float gateHeight = 7.5f;
	float gateDepth = 0.7f;
	float redZoneDepth = 2.0f;  

	
	if (sphereCenter.z > gatePosition.z) {
		
		bool withinWidth = (sphereCenter.x >= gatePosition.x - gateWidth / 2 - sphereRadius) &&
			(sphereCenter.x <= gatePosition.x + gateWidth / 2 + sphereRadius);
		bool withinHeight = (sphereCenter.y >= gatePosition.y - sphereRadius) &&
			(sphereCenter.y <= gatePosition.y + gateHeight + sphereRadius);

		
		bool inRedZone = sphereCenter.z <= (gatePosition.z + redZoneDepth);

	
		if (withinWidth && withinHeight && inRedZone) {
			return true;
		}
	}

	// Verificarea coliziunii cu stâlpii (ace?tia blocheaz? întotdeauna)
	glm::vec3 leftPillarMin = gatePosition + glm::vec3(-gateWidth / 2, 0, -pillarThickness / 2);
	glm::vec3 leftPillarMax = gatePosition + glm::vec3(-gateWidth / 2 + pillarThickness, gateHeight, pillarThickness / 2);

	glm::vec3 rightPillarMin = gatePosition + glm::vec3(gateWidth / 2 - pillarThickness, 0, -pillarThickness / 2);
	glm::vec3 rightPillarMax = gatePosition + glm::vec3(gateWidth / 2, gateHeight, pillarThickness / 2);

	if (SphereAABBCollision(sphereCenter, sphereRadius, leftPillarMin, leftPillarMax)) {
		return true;
	}
	if (SphereAABBCollision(sphereCenter, sphereRadius, rightPillarMin, rightPillarMax)) {
		return true;
	}

	return false;
}

bool Tema2::CheckGatePassthrough(const glm::vec3& sphereCenter, const glm::vec3& gatePosition) {
	float gateWidth = 4.0f;
	float gateHeight = 7.5f;
	float checkDepth = 0.3f;

	
	bool isApproachingFromFront = sphereCenter.z < gatePosition.z;
	bool isNearGatePlane = abs(sphereCenter.z - gatePosition.z) <= checkDepth;
	bool isWithinWidth = (sphereCenter.x >= gatePosition.x - gateWidth / 2) &&
		(sphereCenter.x <= gatePosition.x + gateWidth / 2);
	bool isWithinHeight = (sphereCenter.y >= gatePosition.y) &&
		(sphereCenter.y <= gatePosition.y + gateHeight);

	return isApproachingFromFront && isNearGatePlane && isWithinWidth && isWithinHeight;
}


void Tema2::OnInputUpdate(float deltaTime, int mods) {
	float moveSpeed = 5.0f;
	glm::vec3 newPosition = dronePosition;

	if (window->KeyHold(GLFW_KEY_W)) {
		newPosition.y += moveSpeed * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_S)) {
		newPosition.y -= moveSpeed * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_Q)) {
		newPosition.z -= moveSpeed * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_E)) {
		newPosition.z += moveSpeed * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_A)) {
		newPosition.x -= moveSpeed * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_D)) {
		newPosition.x += moveSpeed * deltaTime;
	}

	// coleziune teren
	if (newPosition.y - droneRadius < -2.0f) {
		newPosition.y = -2.0f + droneRadius;
	}

	// coleziune brazi
	bool collision = false;
	for (const auto& treePos : treePositions) {
		float dx = newPosition.x - treePos.x;
		float dz = newPosition.z - treePos.z;
		float distance = sqrt(dx * dx + dz * dz);

		if (distance < (DRONE_RADIUS + TREE_TRUNK_RADIUS + 1.0f)) {
			collision = true;
			break;
		}
	}

	
	for (const auto& bearPos : bearPositions) {
		float dx = newPosition.x - bearPos.x;
		float dy = newPosition.y - bearPos.y;
		float dz = newPosition.z - bearPos.z;

		
		float distance = sqrt(dx * dx + dy * dy + dz * dz);

		
		float bearCollisionRadius = 2.5f; 

		if (distance < (DRONE_RADIUS + bearCollisionRadius)) {
			collision = true;
			break;
		}

		
		float horizontalDistance = sqrt(dx * dx + dz * dz);
		if (horizontalDistance < 3.0f && abs(dy) < 3.0f) {
			collision = true;
			break;
		}
	}

	if (!collision) {
		dronePosition = newPosition;
		camera->position = dronePosition + glm::vec3(0, cameraHeight, cameraDistance);
	}

	camera->Set(
		camera->position,
		dronePosition,
		glm::vec3(0, 1, 0)
	);
}



void Tema2::OnKeyPress(int key, int mods)
{
	// Add key press event
	if (key == GLFW_KEY_T)
	{
		renderCameraTarget = !renderCameraTarget;
	}
	// TODO(student): Switch projections
	if (key == GLFW_KEY_O)
	{
		perspective = false;
		projectionMatrix = glm::ortho(left, right, bottom, top, znear, zfar);
	}

	if (key == GLFW_KEY_P)
	{
		perspective = true;
		projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, znear, zfar);
	}
}


void Tema2::OnKeyRelease(int key, int mods)
{
	// Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		
		if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL)
		{
			cameraDistance += deltaY * sensivityOY * 10; 
			cameraDistance = glm::clamp(cameraDistance, 2.0f, 10.0f);  

			cameraHeight += deltaX * sensivityOX * 5;    
			cameraHeight = glm::clamp(cameraHeight, 1.0f, 5.0f); 
		}
	}
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button press event
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

