#pragma once
#include <string>
#include "tigl.h"

//TODO Maybe have helper functions so that constructor wont be so full of the implementations?
/*
* The ModelLoader loads the specified models in through the resources path.
*/
class ModelLoader {
public:
	ModelLoader() = default;

	//Loads specified model in .obj format
	void loadModel(const std::string& fileName);

	//After extracting the 3D data, combine it into vertices to render later on.
	std::shared_ptr<tigl::VBO> createVBO();

private:
	//void addPosition();
	//Clears all data of model if already loaded in.
	void clearCache();

	//Stores all the ids that need to be loaded in
	struct Face {
		int position[3];
		int texCoord[3];
		int normal[3];
	};

	//All the data within a .obj file
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords; //For now dont use this, too complicated no model loading yet.
	std::vector<Face> faces;
};