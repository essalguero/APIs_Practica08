#pragma once

#include "Shader.h"

#include "Vertex.h"


#include <vector>

using namespace std;
using namespace glm;

class Buffer
{
public:

	static std::shared_ptr<Buffer> create(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);

	//void draw(const Shader& shader) const;
	void draw(const std::shared_ptr<Shader> & shader) const;

	// Obtiene el mensaje de error generado al crear el buffer
	inline const char*	getError() const { return error; };

	void inverseNormals();

protected:

	static inline void destroy(Buffer* p) {
		delete p;
	}

	~Buffer();
	

	Buffer(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices) : vertices(vertices), indices(indices) {

		glGenBuffers(2, buffer);
		if (buffer[0] == 0 || buffer[1] == 0) {
			std::cout << "could not create buffers" << std::endl;
			return; //return -1
		}

		size_t vsize = vertices.size() * sizeof(Vertex);
		size_t isize = indices.size() * sizeof(uint16_t);

		// Enlazar los buffers de datos autilizar
		glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);

		// Rellenar los buffers de datos. Tienen que estan enlazados previamente
		glBufferData(GL_ARRAY_BUFFER, vsize, vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, indices.data(), GL_STATIC_DRAW);

	};


private:

	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;

	uint32_t buffer[2]{ 0 };

	char error[1024];
};