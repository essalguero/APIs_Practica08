#pragma once
#include "../lib/glm/glm.hpp"
#include "../lib/glew/glew.h"

#include <iostream>
#include <memory>


class Shader
{
public:

	// Devuelve el identificador de OpenGL del programa
	inline uint32_t	getId() const { return id; };

	// Obtiene el mensaje de error generado al compilar o enlazar
	inline const char*	getError() const { return error; };

	// Activa el uso de este programa
	inline void	use() const { glUseProgram(id); } ;

	// Activa la escritura de las variables attribute,
	// y especifica su formato
	void setupAttribs() const;

	// Obtiene la localización de una variable uniform
	int	getLocation(const char* name) const;

	// Obtiene la localización de un bloque
	//int getBlockLocation(const char* name) const;

	// Da valor a una variable uniform
	void setInt(int loc, int val);
	void setFloat(int loc, float val);
	void setVec3(int loc, const glm::vec3& vec);
	void setVec4(int loc, const glm::vec4& vec);
	void setMatrix(int loc, const glm::mat4& matrix);

	static std::shared_ptr<Shader> create(const std::string& vertexCode, const std::string& fragmentCode);

protected:

	Shader(const std::string & stringVertexShader, const std::string & stringFragmentShader);
	~Shader();
	static void destroy(Shader* p) {
		delete p;
	}


private:

	GLuint id{ 0 };

	char error[1024];

	int vposLoc;

	int vTextureLoc;

	int mvpMatrix;

	int vnormalLoc;

	int vtangentLoc;
};

