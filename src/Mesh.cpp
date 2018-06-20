#include "Mesh.h"
#include "State.h"

#include "Material.h"
#include "Texture.h"

#include <sstream>


inline std::string extractPath(std::string filename) {
	while (filename.find('\\') != std::string::npos)
		filename.replace(filename.find('\\'), 1, 1, '/');
	size_t pos = filename.rfind('/');
	if (pos == std::string::npos) return "";
	filename = filename.substr(0, pos);
	if (filename.size() > 0) filename += '/';
	return filename;
}


/*std::vector<std::string> splitString(const std::string& str, char delim) {
	std::vector<std::string> elems;
	std::stringstream sstream(str);
	std::string item;
	if (str != "") {
		while (std::getline(sstream, item, delim)) {
			elems.push_back(item);
		}
	}
	return elems;
}*/

template <typename T>
vector<T> splitStr(const std::string& str, char delim) {
	vector<T> elems;
	stringstream sstream(str);
	string item;
	if (str != "") {
		while (std::getline(sstream, item, delim)) {
			istringstream istream(item);
			T elem;
			istream >> elem;
			elems.push_back(elem);
		}
	}
	return elems;
}


/*template <typename T>
T numberFromString(const std::string& str) {
	T number;
	std::istringstream stream(str);
	stream >> number;
	return number;
}*/

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::addBuffer(const std::shared_ptr<Buffer>& buffer, const Material& material)
{
	buffersVector.push_back(buffer);

	materialsVector.push_back(material);
}

size_t Mesh::getNumBuffers() const
{
	return buffersVector.size();
}

const std::shared_ptr<Buffer>& Mesh::getBuffer(size_t index) const
{
	shared_ptr<Buffer> pointer = nullptr;

	if (index >= 0 && index < buffersVector.size())
		return buffersVector.at(index);

	return pointer;
}

std::shared_ptr<Buffer>& Mesh::getBuffer(size_t index)
{
	shared_ptr<Buffer> pointer = nullptr;

	if (index >= 0 && index < buffersVector.size())
		return buffersVector.at(index);

	return pointer;
}

const Material& Mesh::getMaterial(size_t index) const
{
	return materialsVector.at(index);
}

Material& Mesh::getMaterial(size_t index)
{
	return materialsVector.at(index);
}


void Mesh::draw()
{
	

	for (int i = 0; i < buffersVector.size(); ++i)
	{
		shared_ptr<Shader> shader = materialsVector.at(i).getShader();

		//Activate the shader
		shader->use();

		// Call the material prepare
		materialsVector.at(i).prepare();

		// Draw the buffer using the shader
		buffersVector.at(i)->draw(shader);

	}
}

std::shared_ptr<Mesh> Mesh::load(
	const char* filename,
	const std::shared_ptr<Shader>& shader)
{
	shared_ptr<Mesh> mesh = make_shared<Mesh>();

	bool includesTexture = false;
	bool includesNormals = false;
	bool includesTangents = false;

	bool isCubemap = false;

	//std::shared_ptr<Texture> reflectionTexture;
	//std::shared_ptr<Texture> refractionTexture;
	//std::shared_ptr<Texture> normalTexture;

	bool depthWrite = true;
	Material::BlendMode blendingMode = Material::BlendMode::ALPHA;

	float refractionCoef = 0.0f;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);
	if (result) {
		// Cargado correctamente, podemos analizar su contenido ...
		pugi::xml_node meshNode = doc.child("mesh");

		pugi::xml_node buffersNode = meshNode.child("buffers");

		for (pugi::xml_node bufferNode = buffersNode.child("buffer");
			bufferNode;
			bufferNode = bufferNode.next_sibling("buffer"))
		{
			// Iteramos por todos los buffers
			pugi::xml_node materialNode = bufferNode.child("material");

			std::shared_ptr<Texture> texture;
			std::string textureName = materialNode.child("texture").text().as_string();
			if (textureName != "")
			{
				char delim = ',';
				std::vector<std::string>stringsVector = splitStr<std::string>(textureName, delim);
				if (stringsVector.size() == 6)
				{
					std::string fullTextureName = extractPath(filename);

					std::string leftString = fullTextureName + stringsVector.at(0);
					const char* left = leftString.c_str();
					std::string rightString = fullTextureName + stringsVector.at(1);
					const char* right = rightString.c_str();
					std::string frontString = fullTextureName + stringsVector.at(2);
					const char* front = frontString.c_str();
					std::string backString = fullTextureName + stringsVector.at(3);
					const char* back = backString.c_str();
					std::string topString = fullTextureName + stringsVector.at(4);
					const char* top = topString.c_str();
					std::string bottomString = fullTextureName + stringsVector.at(5);
					const char* bottom = bottomString.c_str();

					texture = Texture::load(left, right, front, back, top, bottom);

					isCubemap = true;
				}
				else
				{

					std::string fullTextureName = extractPath(filename) + textureName;
					texture = Texture::load(fullTextureName.c_str());
				}
				includesTexture = true;
			}


			std::shared_ptr<Texture> normalTexture;
			std::string normalTextureName = materialNode.child("normal_texture").text().as_string();
			if (normalTextureName != "")
			{
				char delim = ',';
				std::vector<std::string>stringsVector = splitStr<std::string>(normalTextureName, delim);
				if (stringsVector.size() == 6)
				{
					std::string fullTextureName = extractPath(filename);

					std::string leftString = fullTextureName + stringsVector.at(0);
					const char* left = leftString.c_str();
					std::string rightString = fullTextureName + stringsVector.at(1);
					const char* right = rightString.c_str();
					std::string frontString = fullTextureName + stringsVector.at(2);
					const char* front = frontString.c_str();
					std::string backString = fullTextureName + stringsVector.at(3);
					const char* back = backString.c_str();
					std::string topString = fullTextureName + stringsVector.at(4);
					const char* top = topString.c_str();
					std::string bottomString = fullTextureName + stringsVector.at(5);
					const char* bottom = bottomString.c_str();

					normalTexture = Texture::load(left, right, front, back, top, bottom);
				}
				else
				{

					std::string fullTextureName = extractPath(filename) + normalTextureName;
					normalTexture = Texture::load(fullTextureName.c_str());
				}
				//includesNormalTexture = true;
			}


			std::shared_ptr<Texture> refractionTexture;
			std::string refractionTextureName = materialNode.child("refract_texture").text().as_string();
			if (refractionTextureName != "")
			{
				char delim = ',';
				std::vector<std::string>stringsVector = splitStr<std::string>(refractionTextureName, delim);
				if (stringsVector.size() == 6)
				{
					std::string fullTextureName = extractPath(filename);

					std::string leftString = fullTextureName + stringsVector.at(0);
					const char* left = leftString.c_str();
					std::string rightString = fullTextureName + stringsVector.at(1);
					const char* right = rightString.c_str();
					std::string frontString = fullTextureName + stringsVector.at(2);
					const char* front = frontString.c_str();
					std::string backString = fullTextureName + stringsVector.at(3);
					const char* back = backString.c_str();
					std::string topString = fullTextureName + stringsVector.at(4);
					const char* top = topString.c_str();
					std::string bottomString = fullTextureName + stringsVector.at(5);
					const char* bottom = bottomString.c_str();

					refractionTexture = Texture::load(left, right, front, back, top, bottom);
				}
				else
				{

					std::string fullTextureName = extractPath(filename) + refractionTextureName;
					refractionTexture = Texture::load(fullTextureName.c_str());
				}
				//includesRefractionTexture = true;
			}

			// Refraction Coefficient
			std::string refractCoefString = materialNode.child("refract_coef").text().as_string();
			if (refractCoefString != "")
			{
				refractionCoef = stof(refractCoefString);
			}


			std::shared_ptr<Texture> reflectionTexture;
			std::string reflectionTextureName = materialNode.child("reflect_texture").text().as_string();
			if (reflectionTextureName != "")
			{
				char delim = ',';
				std::vector<std::string>stringsVector = splitStr<std::string>(reflectionTextureName, delim);
				if (stringsVector.size() == 6)
				{
					std::string fullTextureName = extractPath(filename);

					std::string leftString = fullTextureName + stringsVector.at(0);
					const char* left = leftString.c_str();
					std::string rightString = fullTextureName + stringsVector.at(1);
					const char* right = rightString.c_str();
					std::string frontString = fullTextureName + stringsVector.at(2);
					const char* front = frontString.c_str();
					std::string backString = fullTextureName + stringsVector.at(3);
					const char* back = backString.c_str();
					std::string topString = fullTextureName + stringsVector.at(4);
					const char* top = topString.c_str();
					std::string bottomString = fullTextureName + stringsVector.at(5);
					const char* bottom = bottomString.c_str();

					reflectionTexture = Texture::load(left, right, front, back, top, bottom);
				}
				else
				{

					std::string fullTextureName = extractPath(filename) + reflectionTextureName;
					reflectionTexture = Texture::load(fullTextureName.c_str());
				}
				//includesRefractionTexture = true;
			}


			// Color
			glm::vec4 color = glm::vec4(1.0f);
			std::string colorString = materialNode.child("color").text().as_string();
			if (colorString != "")
			{
				std::vector<float> colorVector = splitStr<float>(colorString, ',');
				color = glm::vec4(colorVector.at(0), colorVector.at(1),
					colorVector.at(2), colorVector.at(3));
			}
			else {
				color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			}


			// shininess
			uint8_t shininess;
			std::string shininessString = materialNode.child("shininess").text().as_string();
			if (shininessString != "")
			{
				shininess = stoi(shininessString);
			}
			else
			{
				shininess = 0;
			}

			std::string depthWriteString = materialNode.child("depthwrite").text().as_string();
			if (depthWriteString != "")
			{
				depthWrite = materialNode.child("depthwrite").text().as_bool();
			}

			std::string blendString = materialNode.child("blend").text().as_string();
			if (blendString != "")
			{
				if (blendString == "alpha")
					blendingMode = Material::BlendMode::ALPHA;
				if (blendString == "add")
					blendingMode = Material::BlendMode::ADD;
				if (blendString == "mul")
					blendingMode = Material::BlendMode::MUL;
			}

			// Read Indices from node
			std::vector<uint16_t> indicesVector;
			std::string indices = bufferNode.child("indices").text().as_string();
			if (indices != "")
			{
				indicesVector = splitStr<uint16_t>(indices, ',');
			}
			else
			{
				return nullptr;
			}

			// Read Coordinates from node
			std::vector<float> coordsVector;
			std::string coords = bufferNode.child("coords").text().as_string();
			if (coords != "")
			{
				coordsVector = splitStr<float>(coords, ',');
				std::cout << "Coordenadas: " << coordsVector.at(0) << std::endl;
			}
			else
			{
				return nullptr;
			}

			// Read Texture Coordinates from node
			std::vector<float> texCoordsVector;
			if (includesTexture)
			{
				std::string texcoords = bufferNode.child("texcoords").text().as_string();
				texCoordsVector = splitStr<float>(texcoords, ',');
			}

			// Read Normals from node
			std::vector<float> normalsVector;
			std::string normalsString = bufferNode.child("normals").text().as_string();
			if (normalsString != "")
			{
				normalsVector = splitStr<float>(normalsString, ',');
				includesNormals = true;
			}

			// Read Normals from node
			std::vector<float> tangentsVector;
			std::string tangentsString = bufferNode.child("tangents").text().as_string();
			if (tangentsString != "")
			{
				tangentsVector = splitStr<float>(tangentsString, ',');
				includesTangents = true;
			}


			auto textCoordsIterator = texCoordsVector.begin();
			auto coordsIterator = coordsVector.begin();
			auto normalsIterator = normalsVector.begin();
			auto tangentsIterator = tangentsVector.begin();

			vector<Vertex> vertexVector;

			for (; coordsIterator != coordsVector.end(); 
				coordsIterator += 3)
			{
				Vertex vertex;

				vertex.position = glm::vec3(*coordsIterator, *(coordsIterator + 1), *(coordsIterator + 2));

				if (includesTexture)
				{
					if (texCoordsVector.size() > 0)
					{
						vertex.texture = glm::vec2(*textCoordsIterator, *(textCoordsIterator + 1));
						textCoordsIterator += 2;
					}
				}
				
				if (includesNormals)
				{
					vertex.normal = glm::vec3(*normalsIterator, *(normalsIterator + 1), *(normalsIterator + 2));
					normalsIterator += 3;
				}

				if (includesTangents)
				{
					vertex.tangent = glm::vec3(*tangentsIterator, *(tangentsIterator + 1), *(tangentsIterator + 2));
					tangentsIterator += 3;
				}

				vertexVector.push_back(vertex);
			}

			Material material = Material::Material(texture, nullptr);

			material.setNormalTexture(normalTexture);
			material.setReflectionTexture(reflectionTexture);
			material.setRefractionTexture(refractionTexture);
			material.setRefractionCoef(refractionCoef);

			material.setShininess(shininess);
			material.setColor(color);
			material.setCulling(true);
			material.setLighting(true);
			material.setBlendMode(blendingMode);
			material.setDepthWrite(depthWrite);


			std::shared_ptr<Buffer> buffer = Buffer::create(vertexVector, indicesVector);

			mesh->addBuffer(buffer, material);
		}

		return mesh;
	}
	else {
		// No se ha podido cargar
		std::cout << result.description() << std::endl;
		return nullptr;
	}
}

