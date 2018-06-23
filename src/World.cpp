#include "World.h"
#include "State.h"

#include <algorithm>

std::string readStringWorld(const char* filename) {
	std::ifstream f(filename, std::ios_base::binary);
	std::stringstream ss;
	ss << f.rdbuf();
	return ss.str();
}

World::World()
{
	glm::vec2 bufferSize = glm::vec2(1024, 1024);
	std::shared_ptr<Texture> depthTexture = Texture::createTexture(bufferSize.x, bufferSize.y, true);
	std::shared_ptr<Framebuffer> depthFramebuffer = std::make_shared<Framebuffer>(nullptr, depthTexture);

	depthCamera = std::make_shared<Camera>(bufferSize);
	depthCamera->setFramebuffer(depthFramebuffer);

	// Store the Shader in the global object State
	std::shared_ptr<Shader> depthShader = Shader::create(readStringWorld("data/shadow_shader.vert"), readStringWorld("data/shadow_shader.frag"));

	// If there  was any error on the generation of the sharder, raise an error
	if (strcmp(depthShader->getError(), "") != 0)
	{
		std::cout << depthShader->getError() << std::endl;
		depthShader = nullptr;
	}
}

void World::addEntity(const std::shared_ptr<Entity>& entity)
{
	std::shared_ptr<Camera> camera = nullptr;
	std::shared_ptr<Light> light = nullptr;

	if (entity == nullptr)
		return;

	// Search if the element is already stored
	std::vector<std::shared_ptr<Entity>>::iterator entityToAdd;

	// Search the element in the vector
	entityToAdd = std::find(entitiesVector.begin(), entitiesVector.end(), entity);

	// If the element was not found, store it
	if (entityToAdd == entitiesVector.end())
	{

		entitiesVector.push_back(entity);

		// Check if it is also a camera
		camera = std::dynamic_pointer_cast<Camera>(entity);
		if (camera != nullptr)
		{
			camerasVector.push_back(camera);
		}
		else
		{
			light = std::dynamic_pointer_cast<Light>(entity);
			if (light != nullptr)
			{
				lightsVector.push_back(light);
			}
		}
	}
}


void World::removeEntity(const std::shared_ptr<Entity>& entity)
{
	if (entity == nullptr)
		return;

	// Iterator to keep the position to delete
	std::vector<std::shared_ptr<Entity>>::iterator entityToDelete;

	// Search the element in the vector
	entityToDelete = std::find(entitiesVector.begin(), entitiesVector.end(), entity);

	// Delete the element if it was found
	if (entityToDelete != entitiesVector.end())
	{
		entitiesVector.erase(entityToDelete);

		// If it is a Camera, also delete it from the camerasVector
		std::shared_ptr<Camera> camera = nullptr;

		camera = std::dynamic_pointer_cast<Camera>(entity);

		if (camera)
		{
			// Iterator to keep the position to delete
			std::vector<std::shared_ptr<Camera>>::iterator cameraToDelete;

			// Search the camera in the vector
			cameraToDelete = std::find(camerasVector.begin(), camerasVector.end(), camera);

			// Delete the element (If it is a camera, it must be also contained in the vector as it is in the entitiesVector)
			camerasVector.erase(cameraToDelete);
		}
		else
		{
			std::shared_ptr<Light> light = nullptr;

			light = std::dynamic_pointer_cast<Light>(entity);
			if (light)
			{
				// Iterator to keep the position to delete
				std::vector<std::shared_ptr<Light>>::iterator lightToDelete;

				// Search the camera in the vector
				lightToDelete = std::find(lightsVector.begin(), lightsVector.end(), light);

				// Delete the element (If it is a camera, it must be also contained in the vector as it is in the entitiesVector)
				lightsVector.erase(lightToDelete);
			}
		}
	}
}


size_t World::getNumEntities() const
{
	return entitiesVector.size();
}


const std::shared_ptr<Entity>& World::getEntity(size_t index) const
{
	std::shared_ptr<Entity> entity = nullptr;

	if (entitiesVector.size() > index) {
		return entitiesVector.at(index);
	}

	return entity;
}


std::shared_ptr<Entity>& World::getEntity(size_t index)
{
	std::shared_ptr<Entity> entity = nullptr;

	if (entitiesVector.size() > index) {
		return entitiesVector.at(index);
	}

	return entity;
}


void World::update(float deltaTime)
{
	for(auto &entity : entitiesVector)
	{
		entity->update(deltaTime);
	}
}


void World::draw()
{

	if (castShadows)
	{
		std::shared_ptr<Light> directionalLight;
		for (int i = 0; i < lightsVector.size(); ++i)
		{
			if (lightsVector.at(i)->getType() == Light::Type::DIRECTIONAL)
			{
				directionalLight = lightsVector.at(i);
				break;
			}
		}

		if (directionalLight)
		{
			State::overrideShader = depthShader;

			glm::vec3 lightPosition = directionalLight->getPosition();
			normalize(lightPosition);

			glm::vec3 halfwayVector = 0.5f * farValue * lightPosition;

			depthCamera->setPosition(halfwayVector);

			glm::vec3 lightDirectionVector = -1.0f * halfwayVector;

			depthCamera->setRotation(glm::vec3(asin(lightDirectionVector.y), atan2(-lightDirectionVector.x, -lightDirectionVector.z), 0));

			depthCamera->setProjection(orthoMatrix);

			depthCamera->prepare();

			depthCamera->draw();

			glm::mat4 biasMatrix = glm::mat4(glm::vec4(0.5f, 0, 0, 0), 
				glm::vec4(0, 0.5f, 0, 0), 
				glm::vec4(0, 0, 0.5f, 0), 
				glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

			State::depthBiasMatrix = biasMatrix * State::projectionMatrix * State::viewMatrix;

			depthCamera->getFramebuffer()->getDepthTexture()->bind(15);

			State::overrideShader = nullptr;

		}
	}

	State::shadows = castShadows;

	State::ambient = ambientLight;
	State::lights = lightsVector;

	for (auto &camera : camerasVector)
	{
		camera->prepare();

		for (auto &entity : entitiesVector)
		{
			entity->draw();
		}
	}
}

const glm::vec3& World::getAmbient() const
{
	return ambientLight;
}
void World::setAmbient(const glm::vec3& ambient)
{
	ambientLight = ambient;
}


void World::setShadows(bool enable)
{
	castShadows = enable;
}

void World::setDepthOrtho(float left, float right,
	float bottom, float top, float near, float far)
{

	orthoMatrix = glm::ortho<float>(left, right, bottom, top, near, far);

	farValue = far;
}