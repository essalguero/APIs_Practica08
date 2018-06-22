#pragma once

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION

#include "common.h"

#include "../lib/glew/glew.h"

class Texture
{
public:
	

	static std::shared_ptr<Texture>	load(const char* filename);
	uint32_t						getId() const;
	const glm::ivec2&				getSize() const;
	void							bind(size_t layer = 0) const;

	static std::shared_ptr<Texture> load(
		const char* left, const char* right,
		const char* front, const char* back,
		const char* top, const char* bottom);
	bool isCube() const;

	static std::shared_ptr<Texture> createTexture(uint16_t width, uint16_t height, bool isDepth = false);
	bool isDepth() const;

	void setDepthTexture(bool isDepth);

protected:
	Texture(GLuint textureId, int height, int width);
	static void destroy(Texture* p) {
		delete p;
	}

	~Texture();

private:
	uint32_t textureId;

	int imageHeight;
	int imageWidth;

	bool isCubemap = false;
	bool isDepthTexture = false;
};