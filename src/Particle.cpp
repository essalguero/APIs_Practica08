#include "Particle.h"

Particle::Particle(
	const Material& mat,
	const glm::vec3& vel,
	float spinVel,
	float lifetime,
	bool autofade) : Billboard(mat)
{
	spin = spinVel;
	remainingLifetime = lifetime;
	totalLifetime = lifetime;
	automaticFade = autofade;
	velocity = vel;
}

float Particle::getRemainingLifetime() const
{
	return remainingLifetime;
}

void Particle::update(float deltaTime)
{
	remainingLifetime -= deltaTime;

	move(velocity * deltaTime);

	rotation = rotation + glm::vec3(0, 0, (spin * deltaTime));

	if (automaticFade)
	{

		float fade;
		if (remainingLifetime > 0.0f)
		{
			fade = remainingLifetime / totalLifetime;
		}
		else
		{
			fade = 0.0f;
		}

		material.setColor(glm::vec4(material.getColor().x, material.getColor().y, material.getColor().z, fade));
	}
}