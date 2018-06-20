#include "Entity.h"

#include "iostream"


const glm::vec3& Entity::getPosition() const
{
	return position;
}

void Entity::setPosition(const glm::vec3& pos)
{
	position = pos;
}

const glm::vec3& Entity::getRotation() const
{
	return glm::degrees(rotation);
}

const void Entity::setRotation(const glm::vec3& rot) 
{
	rotation = glm::radians(rot);
	rotationQuat = glm::quat(rotation);
}


const glm::quat& Entity::getRotationQuat() const 
{
	return rotationQuat;
}

const void Entity::setRotationQuat(const glm::quat& rot) 
{
	rotationQuat = rot;
	rotation = glm::eulerAngles(rot);
}

const glm::vec3& Entity::getScale() const
{
	return scale;
}

void Entity::setScale(const glm::vec3& scale)
{
	this->scale = scale;
}

void Entity::move(const glm::vec3& vec)
{
	position += rotationQuat * vec;
}