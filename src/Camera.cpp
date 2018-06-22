#include "Camera.h"
#include "State.h"


void Camera::prepare()
{
	if (cameraFramebuffer)
	{
		cameraFramebuffer->bind();
	}
	else
	{
		Framebuffer::bindScreen();
	}

	// Activate depth buffer writting
	glDepthMask(true);

	State::projectionMatrix = this->projectionMatrix;
	
	glm::mat4 viewMatrix;

	viewMatrix = glm::translate(
		glm::rotate(glm::mat4(), glm::angle(rotationQuat), -glm::axis(rotationQuat)), 
		-position);

	State::viewMatrix = viewMatrix;

	// Write in the State the position of the camera
	State::eyePos = position;

	glViewport(viewPort.s, viewPort.t, viewPort.p, viewPort.q);
	glScissor(viewPort.s, viewPort.t, viewPort.p, viewPort.q);
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Camera::setFramebuffer(const std::shared_ptr<Framebuffer>& framebuffer)
{
	cameraFramebuffer = framebuffer;
}

const std::shared_ptr<const Framebuffer> Camera::getFramebuffer() const
{
	return cameraFramebuffer;
}