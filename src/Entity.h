#pragma once

#include "common.h"

class Entity
{
public:

	Entity() : scale(glm::vec3(1, 1, 1)) {}
	
	const glm::vec3& getPosition() const;
	void setPosition(const glm::vec3& pos);
	const glm::vec3& getRotation() const;
	const void setRotation(const glm::vec3& rot); // puede ser quat
	const glm::quat& getRotationQuat() const;
	const void setRotationQuat(const glm::quat& rot); // hecho tambien con quat
	const glm::vec3& getScale() const;
	void setScale(const glm::vec3& scale);
	void move(const glm::vec3& vec); // Ver en las diapositivas

	// virtual methods to be redefined in classes inheriting from Entity;
	virtual ~Entity() {};	
	virtual void update(float deltaTime) {};
	virtual void draw() {};

protected:
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(0, 0, 0);

	glm::quat rotationQuat;
};