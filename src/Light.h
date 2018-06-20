#pragma once

#include "common.h"
#include "Entity.h"
#include "Shader.h"

class Light : public Entity
{
	
public:

	enum Type { DIRECTIONAL = 0, POINT };

	Light(glm::vec3 position, Type type, glm::vec3 color, float linearAttenuation, glm::vec3 direction) :
		lightType(type), lightColor(color), linearAttenuation(linearAttenuation) {
		this->position = position;
		this->rotation = direction;
	};

	Type			getType() const;
	void			setType(Type type);
	const glm::vec3& getColor() const;
	void			setColor(const glm::vec3& color);
	float			getLinearAttenuation() const;
	void			setLinearAttenuation(float att);
	void			prepare(int index, std::shared_ptr<Shader>& shader) const;


private:
	Type lightType;
	glm::vec3 lightColor;
	float linearAttenuation;

};