#pragma once

#include "common.h"
#include "Entity.h"

class Camera : public Entity
{
public:
	Camera(const glm::ivec2& viewportSize = glm::ivec2(0, 0)) : viewPort(0, 0, viewportSize.x, viewportSize.y), clearColor(glm::vec3(0, 0, 0)) {
		projectionMatrix = glm::perspective<float>(glm::radians(45.0f), viewPort.y == 0 ? 1 : static_cast<float>(viewportSize.x) / viewportSize.y, 1, 1000);

	}

	const glm::mat4& getProjection() const { return projectionMatrix; };
	void setProjection(const glm::mat4& proj) { projectionMatrix = proj; };
	const glm::ivec4& getViewport() const { return viewPort; };
	void setViewport(const glm::ivec4& vp) { viewPort = vp;  };
	const glm::vec3& getClearColor() const { return viewPort; };
	void setClearColor(const glm::vec3& color) { clearColor = color; };
	void prepare();

private:
	glm::vec3 clearColor;
	glm::ivec4 viewPort;
	glm::mat4 projectionMatrix;
};