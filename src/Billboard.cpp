#include "Billboard.h"
#include "Buffer.h"
#include "Vertex.h"
#include "State.h"

Billboard::Billboard(const Material& mat)
{
	material = mat;
	size = material.getTexture()->getSize();
	material.setDepthWrite(false);

	spin = 0;

	vector<Vertex> vertexVector;
	vector<uint16_t> indicesVector = {0, 1, 2, 2, 3, 0};

	Vertex vertex;
	vertex.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	vertex.position = glm::vec3(-0.5f, -0.5f, 0.0f);
	vertex.texture = glm::vec2(0, 0);
	vertexVector.push_back(vertex);
	vertex.position = glm::vec3(0.5f, -0.5f, 0.0f);
	vertex.texture = glm::vec2(1, 0);
	vertexVector.push_back(vertex);
	vertex.position = glm::vec3(0.5f, 0.5f, 0.0f);
	vertex.texture = glm::vec2(1, 1);
	vertexVector.push_back(vertex);
	vertex.position = glm::vec3(-0.5f, 0.5f, 0.0f);
	vertex.texture = glm::vec2(0, 1);
	vertexVector.push_back(vertex);


	buffer = Buffer::create(vertexVector, indicesVector);

	position = glm::vec3(0.0f, 0.0f, 0.0f);

}

const Material& Billboard::getMaterial() const
{
	return material;
}

Material& Billboard::getMaterial()
{
	return material;
}

const glm::vec2& Billboard::getSize() const
{
	return size;
}

void Billboard::setSize(const glm::vec2& size)
{
	this->size = size;
}

float Billboard::getSpin() const
{
	return spin;
}

void Billboard::setSpin(float spin)
{
	this->spin = spin;
}

void Billboard::draw()
{
	// Get the transpose viewMatrix to generate the model matrix of the Billboard
	glm::mat4 newModelMatrix = glm::transpose(State::viewMatrix);

	// Reinitialize the needed values -> Position is set
	newModelMatrix[0][3] = 0;
	newModelMatrix[1][3] = 0;
	newModelMatrix[2][3] = 0;
	newModelMatrix[3] = glm::vec4(position.x, position.y, position.z, 1.0f);

	// Rotate and Scale the Billboard
	newModelMatrix = glm::rotate(newModelMatrix, glm::radians(spin), glm::vec3(0, 0, 1));
	glm::vec3 billboardScale = glm::vec3(size, 1) * scale;
	newModelMatrix = glm::scale(newModelMatrix, billboardScale);
	State::modelMatrix = newModelMatrix;
	
	//Set the parameters to draw the Billboard
	material.prepare();
	std::shared_ptr<Shader> shader = material.getShader();
	buffer->draw(shader);
}