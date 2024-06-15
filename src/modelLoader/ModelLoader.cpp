#include "ModelLoader.h"
#include <sstream>
#include <fstream>


//TODO Line seperator for easier lookup of data.
//TODO Try to load in suzanne as a test
//TODO ModelManager so that we can cache the VBO's created.
//TODO ModelManager needs unordered map of models <objname, tigl::VBO>

void ModelLoader::loadModel(const std::string& fileName) {
	clearCache();
	std::ifstream fileStream(fileName);
	if (!fileStream.is_open()) {
		throw std::invalid_argument("File either not found, or invalid path!");
	}

	std::string line;
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		std::stringstream strStream(line);

		std::string prefix;
		strStream >> prefix;

		if (prefix == "v") {
			glm::vec3 position(0, 0, 0);
			strStream >> position.x;
			strStream >> position.y;
			strStream >> position.z;
			this->positions.push_back(position);
		}
		else if (prefix == "vn") {
			glm::vec3 normal(0, 0, 0);
			strStream >> normal.x;
			strStream >> normal.y;
			strStream >> normal.z;
			this->normals.push_back(normal);
		}
		else if (prefix == "vt") {
			glm::vec2 texCoord(0, 0);
			strStream >> texCoord.x;
			strStream >> texCoord.y;
			this->textureCoords.push_back(texCoord);
		}
		else if (prefix == "f") {

            //TODO Make a string splitter for the face data.
			Face face{};
			for (int i = 0; i < 3; i++)
			{
				char junk = ' ';
				strStream >> face.position[i];
				strStream >> junk;
				strStream >> face.texCoord[i];
				strStream >> junk;
				strStream >> face.normal[i];

				//-1 Because vector index starts at 0, but face id starts at 1.
				face.position[i]--;
				face.texCoord[i]--;
				face.normal[i]--;
			}
			this->faces.push_back(face);
		}
	}
	fileStream.close();
}

std::shared_ptr<tigl::VBO> ModelLoader::createVBO() {
	std::vector<tigl::Vertex> vertices;

	//Load everything into the vertex;
	//TODO Maybe split for loops?
	for (const auto& face : this->faces) {
		for (int i = 0; i < 3; i++) {
			auto& position = this->positions[face.position[i]];
			auto& normal = this->normals[face.normal[i]];
			auto& texCoord = this->textureCoords[face.texCoord[i]];

			vertices.push_back(tigl::Vertex::PCTN(position, glm::vec4(1.f), texCoord, normal));
		}
	}

    std::shared_ptr<tigl::VBO> modelVBO;
    modelVBO.reset(tigl::createVbo(vertices));

	vertices.clear();
	return modelVBO;
}

void ModelLoader::clearCache()
{
	this->positions.clear();
	this->normals.clear();
	this->textureCoords.clear();
	this->faces.clear();
}
