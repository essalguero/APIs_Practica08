#include "Material.h"
#include "State.h"

Material::Material(const std::shared_ptr<Texture>& tex, 
	const std::shared_ptr<Shader>& shader)
{
	materialTexture = tex;
	materialShader = shader;

	materialColor = glm::vec4(1.0f);
}

const std::shared_ptr<Shader>& Material::getShader() const
{
	if (!materialShader)
		return State::defaultShader;

	return materialShader;
}

std::shared_ptr<Shader>& Material::getShader()
{
	if (!materialShader)
		return State::defaultShader;

	return materialShader;
}

void Material::setShader(const std::shared_ptr<Shader>& shader)
{
	materialShader = shader;
}

const std::shared_ptr<Texture>& Material::getTexture() const
{
	return materialTexture;
}

void Material::setTexture(const std::shared_ptr<Texture>& tex)
{
	materialTexture = tex;
}

void Material::prepare()
{ 
	std::shared_ptr<Shader> shader = getShader();

	shader->use();


	glm::mat4 mvMatrix = State::viewMatrix * State::modelMatrix;
	glm::mat4 mvpMatrix = State::projectionMatrix * mvMatrix;

	glm::mat4 normalsMatrix(mvMatrix);
	normalsMatrix = glm::transpose(glm::inverse(normalsMatrix));


	shader->setMatrix(shader->getLocation("mvMatrix"), mvMatrix);
	shader->setMatrix(shader->getLocation("normalsMatrix"), normalsMatrix);
	shader->setMatrix(shader->getLocation("mvpMatrix"), mvpMatrix);

	shader->setMatrix(shader->getLocation("ModelMatrix"), State::modelMatrix);

	shader->setVec3(shader->getLocation("eyePos"), State::eyePos);

	// Set other variables
	int isTexturizedLoc = getShader()->getLocation("isTexturized");
	int hasColorLoc = getShader()->getLocation("hasColor");
	
	int colorLoc = getShader()->getLocation("color");

	// Check if there is a texture to be used
	if (isTexturizedLoc != -1)
	{

		if (materialTexture || normalTexture || reflectionTexture || refractionTexture)
		{
			shader->setInt(isTexturizedLoc, 1);
		}
		else
		{
			shader->setInt(isTexturizedLoc, 0);
		}
	}

	if (materialTexture)
	{
		shader->setInt(getShader()->getLocation("hasTexture"), 1);

		if (materialTexture->isCube())
		{
			materialTexture->bind(0 + 4);
			int location = getShader()->getLocation("texSamplerCube");
			shader->setInt(location, 0 + 4);
			location = getShader()->getLocation("isCubemap");
			shader->setInt(location, 1);
		}
		else
		{
			materialTexture->bind(0);
			int location = getShader()->getLocation("texSampler");
			shader->setInt(location, 0);
			location = getShader()->getLocation("isCubemap");
			shader->setInt(location, 0);
		}
	}
	else
	{
		shader->setInt(getShader()->getLocation("hasTexture"), 0);
	}

	if (normalTexture)
	{
		int location = getShader()->getLocation("hasNormalTexture");
		shader->setInt(location, 1);
		if (normalTexture->isCube())
		{
			normalTexture->bind(1 + 4);
			int location = getShader()->getLocation("normalTextureCube");
			shader->setInt(location, 1 + 4);
			location = getShader()->getLocation("isCubemap");
			shader->setInt(location, 1);
		}
		else
		{
			normalTexture->bind(1);
			int location = getShader()->getLocation("normalTexture");
			shader->setInt(location, 1);
			location = getShader()->getLocation("isCubemap");
			shader->setInt(location, 0);
		}
	}
	else
	{
		shader->setInt(getShader()->getLocation("hasNormalTexture"), 0);
	}

	if (reflectionTexture)
	{
		int location = getShader()->getLocation("hasReflectionTexture");
		shader->setInt(location, 1);
		if (reflectionTexture->isCube())
		{
			reflectionTexture->bind(2 + 4);
			shader->setInt(getShader()->getLocation("reflectionTextureCube"), 2 + 4);
			location = getShader()->getLocation("isCubemap");
			shader->setInt(location, 1);
		}
		else
		{
			reflectionTexture->bind(2);
			shader->setInt(getShader()->getLocation("reflectionTexture"), 2);
			location = getShader()->getLocation("isCubemap");
			shader->setInt(location, 0);
		}
	}
	else
	{
		shader->setInt(getShader()->getLocation("hasReflectionTexture"), 0);
	}


	shader->setFloat(getShader()->getLocation("refractionCoef"), refractionCoef);
	if (refractionTexture)
	{
		int location = getShader()->getLocation("hasRefractionTexture");

		shader->setInt(location, 1);
		if (refractionTexture->isCube())
		{
			refractionTexture->bind(3 + 4);
			shader->setInt(getShader()->getLocation("refractionTextureCube"), 3 + 4);

			location = getShader()->getLocation("isCubemap");
			shader->setInt(location, 1);
		}
		else
		{
			refractionTexture->bind(3);
			shader->setInt(getShader()->getLocation("refractionTexture"), 3);
			location = getShader()->getLocation("isCubemap");
			shader->setInt(location, 0);
		}
	}
	else
	{
		shader->setInt(getShader()->getLocation("hasRefractionTexture"), 0);
	}

	// Check if there is a texture to be used
	if (hasColorLoc != -1)
	{
		shader->setInt(hasColorLoc, 1);
		shader->setVec4(colorLoc, materialColor);
	}

	int variableLocation = shader->getLocation("numberLights");
	if (lighting)
	{
		shader->setInt(variableLocation, static_cast<int>(State::lights.size()));

		variableLocation = shader->getLocation("shininess");
		shader->setInt(variableLocation, materialShininess);
		variableLocation = shader->getLocation("ambientLight");
		shader->setVec3(variableLocation, State::ambient);

		for (int i = 0; i < State::lights.size(); ++i)
		{
			State::lights.at(i)->prepare(i, shader);
		}
	}
	else
	{
		shader->setInt(variableLocation, 0);
	}
	variableLocation = shader->getLocation("diffuse");
	shader->setVec4(variableLocation, materialColor);

	//Set blending mode
	switch (blendingMode) {
		case Material::BlendMode::ALPHA:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case Material::BlendMode::ADD:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		case Material::BlendMode::MUL:
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			break;
		default:
			break;
	}

	// Set culling mode
	(culling) ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

	// Set Depth buffer active/non active
	glDepthMask(depthWrite);
}

const glm::vec4& Material::getColor() const
{
	return materialColor;
}

void Material::setColor(const glm::vec4& color)
{
	materialColor = color;
}

uint8_t Material::getShininess() const
{
	return materialShininess;
}

void Material::setShininess(uint8_t shininess)
{
	materialShininess = shininess;
}

Material::BlendMode	Material::getBlendMode() const
{
	return blendingMode;
}

void Material::setBlendMode(Material::BlendMode blendMode)
{
	blendingMode = blendMode;
}

bool Material::getLighting() const
{
	return lighting;
}

void Material::setLighting(bool enable)
{
	lighting = enable;
}

bool Material::getCulling() const
{
	return culling;
}

void Material::setCulling(bool enable)
{
	culling = enable;
}

bool Material::getDepthWrite() const
{
	return depthWrite;
}

void Material::setDepthWrite(bool enable)
{
	depthWrite = enable;
}


const std::shared_ptr<Texture>& Material::getReflectionTexture() const
{
	return reflectionTexture;
}

void Material::setReflectionTexture(const std::shared_ptr<Texture>& tex)
{
	reflectionTexture = tex;
}

const std::shared_ptr<Texture>& Material::getRefractionTexture() const
{
	return refractionTexture;
}

void Material::setRefractionTexture(const std::shared_ptr<Texture>& tex)
{
	refractionTexture = tex;
}

const std::shared_ptr<Texture>& Material::getNormalTexture() const
{
	return normalTexture;
}

void Material::setNormalTexture(const std::shared_ptr<Texture>& tex)
{
	normalTexture = tex;
}

float Material::getRefractionCoef() const
{
	return refractionCoef;
}

void Material::setRefractionCoef(float coef)
{
	refractionCoef = coef;
}

