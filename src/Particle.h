#pragma once

#include "common.h"
#include "Material.h"
#include "Billboard.h"

class Particle : public Billboard
{
public:
	Particle(
		const Material& mat,
		const glm::vec3& vel,
		float spinVel,
		float lifetime,
		bool autofade);
	float getRemainingLifetime() const;
	virtual void update(float deltaTime) override;

private:
	float remainingLifetime;
	float totalLifetime;
	bool automaticFade;
};