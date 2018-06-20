#include "Framebuffer.h"

Framebuffer::Framebuffer(
	const std::shared_ptr<Texture>& colorTex,
	const std::shared_ptr<Texture>& depthTex)
{
	colorTexture = colorTex;
	depthTexture = depthTex;

	glGenFramebuffers(1, &idFrameBuffer);
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
}

void Framebuffer::bindScreen()
{
	//glBindFramebuffer(GL_FRAMEBUFFER, idFrameBuffer);
}