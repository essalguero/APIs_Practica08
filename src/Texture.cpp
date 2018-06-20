#include "Texture.h"
#include "../lib/stb-image/stb_image.h"

#include "common.h"

std::shared_ptr<Texture> Texture::load(const char* filename)
{
	//std::shared_ptr<Texture> texture;

	int imageHeight;
	int imageWidth;
	GLuint textureId;
	
	// Flip image as the image in files is stored in top/bottom order
	// while OpenGL is expecting the file to be in bottom/up order
	stbi_set_flip_vertically_on_load(true);
	
	stbi_uc* stbiImageLoaded = stbi_load(filename, &imageHeight, &imageWidth, nullptr, 4);

	if (!stbiImageLoaded)
		return nullptr;

	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, stbiImageLoaded);

	glGenerateMipmap(GL_TEXTURE_2D);
	

	std::shared_ptr<Texture> texture(new Texture(textureId,imageHeight, imageWidth), destroy);

	stbi_image_free(stbiImageLoaded);

	return texture;
}

Texture::Texture(GLuint textureId, int height, int width) : textureId(textureId), imageHeight(height), imageWidth(width) {}

uint32_t Texture::getId() const
{
	return textureId;
}

const glm::ivec2& Texture::getSize() const
{
	glm::ivec2 size = glm::ivec2(imageWidth, imageHeight);
	return size;
}

void Texture::bind(size_t layer) const
{
	glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(layer));

	if (isCubemap)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, textureId);
	}
}

std::shared_ptr<Texture> Texture::load(
	const char* left, const char* right,
	const char* front, const char* back,
	const char* top, const char* bottom)
{
	int imageHeight;
	int imageWidth;
	GLuint textureId;

	// Flip image as the image in files is stored in top/bottom order
	// while OpenGL is expecting the file to be in bottom/up order
	//stbi_set_flip_vertically_on_load(true);


	

	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);


	stbi_uc* stbiImageLoaded = stbi_load(right, &imageHeight, &imageWidth, nullptr, 4);
	if (!stbiImageLoaded)
		return nullptr;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, stbiImageLoaded);
	stbi_image_free(stbiImageLoaded);


	stbiImageLoaded = stbi_load(left, &imageHeight, &imageWidth, nullptr, 4);
	if (!stbiImageLoaded)
		return nullptr;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, stbiImageLoaded);
	stbi_image_free(stbiImageLoaded);


	stbiImageLoaded = stbi_load(top, &imageHeight, &imageWidth, nullptr, 4);
	if (!stbiImageLoaded)
		return nullptr;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, stbiImageLoaded);
	stbi_image_free(stbiImageLoaded);


	stbiImageLoaded = stbi_load(bottom, &imageHeight, &imageWidth, nullptr, 4);
	if (!stbiImageLoaded)
		return nullptr;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, stbiImageLoaded);
	stbi_image_free(stbiImageLoaded);


	stbiImageLoaded = stbi_load(front, &imageHeight, &imageWidth, nullptr, 4);
	if (!stbiImageLoaded)
		return nullptr;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, stbiImageLoaded);
	stbi_image_free(stbiImageLoaded);


	stbiImageLoaded = stbi_load(back, &imageHeight, &imageWidth, nullptr, 4);
	if (!stbiImageLoaded)
		return nullptr;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, stbiImageLoaded);
	stbi_image_free(stbiImageLoaded);


	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	std::shared_ptr<Texture> texture(new Texture(textureId, imageHeight, imageWidth), destroy);

	texture->isCubemap = true;

	return texture;
}

bool Texture::isCube() const
{
	return isCubemap;
}