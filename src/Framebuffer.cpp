#include "Framebuffer.h"

Framebuffer::Framebuffer(
	const std::shared_ptr<Texture>& colorTex,
	const std::shared_ptr<Texture>& depthTex)
{
	colorTexture = colorTex;
	depthTexture = depthTex;

	glGenFramebuffers(1, &idFrameBuffer);

	if (colorTexture)
	{
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTexture->getId(), 0);
	}

	if (depthTexture)
	{
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture->getId(), 0);
	}

}

Framebuffer::~Framebuffer()
{
	colorTexture = nullptr;
	depthTexture = nullptr;

	glDeleteFramebuffers(1, &idFrameBuffer);
}


const std::shared_ptr<Texture>& Framebuffer::getColorTexture() const
{
	return colorTexture;
}

const std::shared_ptr<Texture>& Framebuffer::getDepthTexture() const
{
	return depthTexture;
}


void Framebuffer::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, idFrameBuffer);

	if (!colorTexture)
		glDrawBuffer(GL_NONE);
}

void Framebuffer::bindScreen()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}