#pragma once

#include "common.h"
#include "Entity.h"
#include "Mesh.h"


class Model : public Entity
{
public:

	Model(const std::shared_ptr<Mesh>& mesh);
	virtual void draw() override;
	//virtual void update(float deltaTime) override;
	
private:
	shared_ptr<Mesh> mesh;
};