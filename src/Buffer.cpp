#include "Buffer.h"

#include <memory>

Buffer::~Buffer() {
	glDeleteBuffers(2, buffer);
}

std::shared_ptr<Buffer> Buffer::create(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
{
	std::shared_ptr<Buffer> p(new Buffer(vertices, indices), destroy);
	if (strcmp(p->error, "") != 0) {
		p = nullptr;
	}
	return p;
}

void Buffer::inverseNormals() {
	for (int i = 0; i < vertices.size(); ++i)
	{
		vertices.at(i).normal = vertices.at(i).normal * glm::vec3(-1.0f);
	}
}

void Buffer::draw(const std::shared_ptr<Shader> & shader) const {
	// Volver a enlazar los buffers de datos antes de usarlos
	// Los datos ya se cargaron en los buffers en el Constructor
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);

	shader->setupAttribs();

	// draw the data in the buffer
	glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()), GL_UNSIGNED_SHORT, nullptr);
}
