/*#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif*/


#include <fstream>
#include <iostream>
#include <sstream>

#include <vector>

#include <memory>


#include "../lib/glew/glew.h"
#include "../lib/glfw3/glfw3.h"
#include "common.h"
#include "Shader.h"
#include "Vertex.h"
#include "Buffer.h"
#include "Mesh.h"
#include "Entity.h"
#include "Model.h"
#include "Camera.h"
#include "World.h"
#include "Texture.h"
#include "Emitter.h"

#include "State.h"


#define FULLSCREEN false

const float ROTATION_SPEED = 0.02f;
const float MOVING_SPEED = 1.0f;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

std::string readString(const char* filename) {
	std::ifstream f(filename, std::ios_base::binary);
	std::stringstream ss;
	ss << f.rdbuf();
	return ss.str();
}


int init() {
	
	// init glew
	if (glewInit()) {
		std::cout << "could not initialize glew" << std::endl;
		return -1;
	}

	// enable gl states
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_BLEND);
	return 0;

}

void configureEmitter(std::shared_ptr<Emitter>& emitter, glm::vec4 minColorRange, glm::vec4 maxColorRange,
	float minLifetimeRange, float maxLifetimeRange, float minRateRange, float maxRateRange,
	float minScaleRange, float maxScaleRange, glm::vec3 minVelocityRange, glm::vec3 maxVelocityRange,
	float minSpinRange, float maxSpinRange, bool emitting)
{
	emitter->setColorRange(minColorRange, maxColorRange);
	emitter->setLifetimeRange(minLifetimeRange, maxLifetimeRange);
	emitter->setRateRange(minRateRange, maxRateRange);
	emitter->setScaleRange(minScaleRange, maxScaleRange);
	emitter->setVelocityRange(minVelocityRange, maxVelocityRange);
	emitter->setSpinVelocityRange(minSpinRange, maxSpinRange);
	emitter->emit(emitting);
}


// This method creates all the models and add them to the world
int createModelsInWorld(World & world, std::vector<Emitter>& emittersVector)
{
	// Load skybox model from file
	std::shared_ptr<Mesh>skyboxMesh = Mesh::load("data/skybox.msh.xml");

	if (skyboxMesh == nullptr)
		return 0;

	// Create model
	shared_ptr<Model> skyboxModel = make_shared<Model>(skyboxMesh);
	//skyboxModel->setScale(vec3(20.0f, 20.0f, 20.0f));
	skyboxModel->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	//skyboxMesh->getMaterial(0).setLighting(false);

	// Add model
	world.addEntity(skyboxModel);


	// Load Suzanne model from file
	std::shared_ptr<Mesh> suzanneMesh = Mesh::load("data/suzanne_refract.msh.xml");

	if (suzanneMesh == nullptr)
	return 0;

	// Create model
	shared_ptr<Model> suzanneModel = make_shared<Model>(suzanneMesh);
	suzanneModel->setScale(vec3(1.0f, 1.0f, 1.0f));
	suzanneModel->setPosition(glm::vec3(-4.0f, 0.0f, 0.0f));

	// Add model
	world.addEntity(suzanneModel);


	// Load teapot model from file
	std::shared_ptr<Mesh> teapotMesh = Mesh::load("data/teapot_reflect.msh.xml");

	if (teapotMesh == nullptr)
		return 0;

	// Create model
	shared_ptr<Model> teapotModel = make_shared<Model>(teapotMesh);
	teapotModel->setScale(vec3(1.0f, 1.0f, 1.0f));
	teapotModel->setPosition(glm::vec3(4.0f, 0.0f, 0.0f));

	// Add model
	world.addEntity(teapotModel);


	// Load the cube model from file
	std::shared_ptr<Mesh> cubeMesh = Mesh::load("data/cube.msh.xml");
	//cubeMesh->getMaterial(0).setNormalTexture(nullptr);

	if (cubeMesh == nullptr)
		return 0;

	// Create model - Column
	shared_ptr<Model> cubeModel = make_shared<Model>(cubeMesh);
	//cubeModel->setScale(vec3(0.01f, 0.01f, 0.01f));
	cubeModel->setPosition(glm::vec3(0.0f, 0.0f, -4.0f));

	// Add model - Column
	world.addEntity(cubeModel);

	

	// Set Lighting
	world.setAmbient(glm::vec3(0.1, 0.1, 0.1));
	std::shared_ptr<Light>pointLight = std::make_shared<Light>(vec3(1.0f, 1.0f, 1.0f), Light::Type::POINT,
		glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	world.addEntity(pointLight);

	return 1;
}


int main(int, char**) {

	//Camera rotation is update in every frame
	float angle = 0.0f;

	std::vector<Emitter> emittersVector;
	if (glfwInit() != GLFW_TRUE) {
		std::cout << "could not initalize glfw" << std::endl;
		return -1;
	}
	atexit(glfwTerminate);

	// create window
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "U-gine", FULLSCREEN ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	if (!window) {
		std::cout << "could not create glfw window" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (init())
		return -1;


	// Store the Shader in the global object State
	State::defaultShader = Shader::create(readString("data/shader.vert"), readString("data/shader.frag"));

	// If there  was any error on the generation of the sharder, raise an error
	if (strcmp(State::defaultShader->getError(), "") != 0)
	{
	cout << State::defaultShader->getError() << endl;
		return -1;
	}

	// Generate the world
	World world;

	// Generate a camera and store it in the world
	shared_ptr<Camera> camera = make_shared<Camera>();
	camera->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	camera->setRotation(glm::vec3(0, 0, 0));
	camera->setClearColor(glm::vec3(0.0f, 0.0f, 0.0f));
	world.addEntity(camera);

	// Generate the objects in the world
	if (!createModelsInWorld(world, emittersVector))
	{
		cout << "Error creating the Model objects in the world" << endl;
		return -1;
	}

	double xPrev;
	double yPrev;

	glfwGetCursorPos(window, &xPrev, &yPrev);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Bucle principal
	float lastTime = static_cast<float>(glfwGetTime());
	while ( !glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE) ) {

		// update delta time
		float newTime = static_cast<float>(glfwGetTime());
		float deltaTime = newTime - lastTime;
		lastTime = newTime;


		// Check key status
		/*if (glfwGetKey(window, GLFW_KEY_W) || glfwGetKey(window, GLFW_KEY_UP))
		{
			camera->move(vec3(0.0f, 0.0f, -MOVING_SPEED) * deltaTime);
		}

		if (glfwGetKey(window, GLFW_KEY_S) || glfwGetKey(window, GLFW_KEY_DOWN))
		{
			camera->move(vec3(0.0f, 0.0f, MOVING_SPEED) * deltaTime);
		}

		if (glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT))
		{
			camera->move(vec3(-MOVING_SPEED, 0.0f, 0.0f) * deltaTime);
		}

		if (glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT))
		{
			camera->move(vec3(MOVING_SPEED, 0.0f, 0.0f) * deltaTime);
		}

		//Check mouse position
		glfwGetCursorPos(window, &xCurrent, &yCurrent);

		glm::vec3 newRotation = glm::vec3((yPrev - yCurrent), (xPrev - xCurrent), 0.0f) * ROTATION_SPEED;
		glm::vec3 currentRot = camera->getRotation();
		camera->setRotation((currentRot + newRotation));
		yPrev = yCurrent;
		xPrev = xCurrent;*/
		

		// get updated screen size
		int screenWidth, screenHeight;
		glfwGetWindowSize(window, &screenWidth, &screenHeight);

		// report screen size
		std::stringstream ss;
		ss << screenWidth << "x" << screenHeight;
		glfwSetWindowTitle(window, ss.str().c_str());

		// Update viewport in case the screen has been resized
		camera->setViewport(glm::ivec4(0, 0, screenWidth, screenHeight));

		//Camera rotation
		camera->setPosition(glm::vec3(0, 0, 0));
		angle += 32 * deltaTime;
		camera->setRotation(glm::vec3(0, angle, 0));
		camera->move(glm::vec3(0, 0, 10));
		camera->setPosition(camera->getPosition());

		// Set position para el skybox y para la luz
		world.getEntity(1)->setPosition(camera->getPosition());
		world.getEntity(world.getNumEntities() - 1)->setPosition(camera->getPosition());

		// Set projection matrix in case the screen has been resized
		glm::mat4 projectionMatrix = glm::perspective(45.0f,
			static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);
		camera->setProjection(projectionMatrix);

		
		// Draw the objects
		world.update(deltaTime);
		world.draw();

		// update swap chain & process events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	return 0;
}