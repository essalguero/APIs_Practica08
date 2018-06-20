#include "Model.h"
#include "State.h"

#include "common.h"

Model::Model(const std::shared_ptr<Mesh>& mesh)
{
	this->mesh = mesh;
}

void Model::draw()
{
	// Calculate the model matrix
	State::modelMatrix = glm::translate(glm::mat4(), position);
	State::modelMatrix = glm::rotate(State::modelMatrix, glm::angle(rotationQuat), glm::axis(rotationQuat));
	State::modelMatrix = glm::scale(State::modelMatrix, scale);

	// Draw the mesh
	mesh->draw();
}