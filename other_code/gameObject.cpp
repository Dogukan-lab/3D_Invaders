#include "GameObject.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#include "Material.h"
#include "tigl.h"

//Constructors
GameObject::GameObject(const glm::vec3 trans)
{
	transform = trans;
}

GameObject::GameObject(const std::string& dir, const std::string& fileName)
{
	LoadObjectFile(dir, fileName);
}

//Destructor
GameObject::~GameObject(void) = default;

//Draw the object
void GameObject::Draw() const
{
	//All vertices are grouped by their unique mtl
	for (const auto group : groups)
	{
		//Start drawing
		tigl::begin(GL_TRIANGLES);

		//Select the groups mtl
		materials[group->materialIndex]->SelectMaterial();

		//A group can contain multiple faces, go through each
		for (const auto& face : group->faces)
		{
			//Draw each vertex on the face
			for (const auto indexedVertex : face)
			{
				addVertex(tigl::Vertex::PTN(vertices[indexedVertex.pos] + transform,
				textureCoords[indexedVertex.texture],
				normals[indexedVertex.normal]
				));
			}
		}

		//Stop drawing
		tigl::end();
	}
}

//Load an object file with the specified directory and filename
void GameObject::LoadObjectFile(const std::string& dir, const std::string& fileName)
{
	std::string fullDir = dir + "/objects/" + fileName + "/" + fileName + ".obj";

	//Clean fileName
	std::cout << "Loading " << fullDir << std::endl;
	std::string dirName = fullDir;
	if (dirName.rfind('/') != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind('/'));
	if (dirName.rfind('\\') != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind('\\'));
	if (fullDir == dirName)
		dirName = "";

	//Open obj file
	std::ifstream pFile(fullDir.c_str());

	if (!pFile.is_open())
	{
		std::cout << "Could not open file " << fullDir << std::endl;
		return;
	}

	//Start new mtl group
	auto* currentGroup = new ObjectGroup();
	currentGroup->materialIndex = -1;

	//Read all lines in the file
	while (!pFile.eof())
	{
		std::string line;
		std::getline(pFile, line);
		line = CleanLine(line);
		if (line.empty() || line[0] == '#')
			continue;

		//Split parameters into separate strings
		std::vector<std::string> params = Split(line, " ");
		params[0] = ToLower(params[0]);

		//Vetrex
		if (params[0] == "v")
			vertices.push_back(glm::vec3((float)atof(params[1].c_str()), (float)atof(params[2].c_str()),
				(float)atof(params[3].c_str())));
		//Vertex normal
		else if (params[0] == "vn")
			normals.push_back(glm::vec3((float)atof(params[1].c_str()), (float)atof(params[2].c_str()),
				(float)atof(params[3].c_str())));

		//Vertex texture coordinate
		else if (params[0] == "vt")
			textureCoords.push_back(glm::vec2((float)atof(params[1].c_str()), (float)atof(params[2].c_str())));

		//Face
		else if (params[0] == "f")
		{
			//All shapes
			for (size_t ii = 4; ii <= params.size(); ii++)
			{
				std::vector<IndexedVertex> face;

				for (size_t i = ii - 3; i < ii; i++)
				{
					IndexedVertex vertex = IndexedVertex();
					std::vector<std::string> indices = Split(params[i == (ii - 3) ? 1 : i], "/");
					if (indices.size() >= 1)
						vertex.pos = atoi(indices[0].c_str()) - 1;
					if (indices.size() == 2)
						vertex.texture = atoi(indices[1].c_str()) - 1;
					if (indices.size() == 3)
					{
						if (!indices[1].empty())
							vertex.texture = atoi(indices[1].c_str()) - 1;
						vertex.normal = atoi(indices[2].c_str()) - 1;
					}
					face.push_back(vertex);
				}
				currentGroup->faces.push_back(face);
			}

			//Triangle only
			//Create vertex list
			// std::vector<IndexedVertex> face = std::vector<IndexedVertex>();
			//
			// //For each parameter
			// for (size_t i = 1; i < params.size(); i++)
			// {
			// 	//Create new vertex
			// 	IndexedVertex vertex = IndexedVertex();
			//
			// 	//Split position, normal and texture coordinate
			// 	std::vector<std::string> indices = Split(params[i], "/");
			//
			// 	//Assign values
			// 	vertex.pos = atoi(indices[0].c_str()) - 1;
			// 	vertex.texture = atoi(indices[1].c_str()) - 1;
			// 	vertex.normal = atoi(indices[2].c_str()) - 1;
			//
			// 	//Add vertex to face
			// 	face.push_back(vertex);
			// }
			// //Add face to group
			// currentGroup->faces.push_back(face);
		}

		//Load material file
		else if (params[0] == "mtllib")
		{
			std::string mtlPath = fullDir;
			Replace(mtlPath, ".obj", ".mtl");

			LoadMaterialFile(mtlPath);
		}

		//Switch to a new group
		else if (params[0] == "usemtl")
		{
			//Save group that was last being worked on
			if (!currentGroup->faces.empty())
				groups.push_back(currentGroup);

			//Create new group
			currentGroup = new ObjectGroup();
			currentGroup->materialIndex = -1;

			//Search for the corresponding mtl using the name
			for (size_t i = 0; i < materials.size(); i++)
			{
				Material* mat = materials[i];
				if (mat->GetName() == params[1])
				{
					//Save mtl values for the group
					currentGroup->materialIndex = i;
					currentGroup->name = mat->GetName();
					break;
				}
			}

			//Error handle 
			if (currentGroup->materialIndex == -1)
				std::cout << "Could not find material name " << params[1] << std::endl;
		}
	}

	//Add group to list of groups
	groups.push_back(currentGroup);
}

//Load a material file with the specified name and path
void GameObject::LoadMaterialFile(const std::string& mtlPath)
{
	//Open the file
	std::cout << "Loading " << mtlPath << std::endl;
	std::ifstream pFile(mtlPath);
	if (!pFile.is_open())
	{
		std::cout << "Could not open file " << mtlPath << std::endl;
		return;
	}

	//Read all lines and write the data to the material file
	Material* currentMaterial = nullptr;
	while (!pFile.eof())
	{
		//Save and clean current line
		std::string line;
		std::getline(pFile, line);
		line = CleanLine(line);

		//Ignore empty and commented lines
		if (line == "" || line[0] == '#')
			continue;

		//Split parameters into separate strings
		std::vector<std::string> params = Split(line, " ");
		params[0] = ToLower(params[0]);

		if (params[0] == "newmtl")	//Create new mtl
		{
			//Save the mtl that was being worked on
			if (currentMaterial != nullptr)
			{
				materials.push_back(currentMaterial);
			}

			//Create new mtl
			currentMaterial = new Material(params[1]);
		}
		else if (params[0] == "map_kd")	//texture diffuse (normal color)
		{
			//Make sure the path is in the same directory as the mtl file
			std::string tex = params[1];
			if (tex.find("/"))
				tex = tex.substr(tex.rfind("/") + 1);
			if (tex.find("\\"))
				tex = tex.substr(tex.rfind("\\") + 1);

			//Save the texture path to the mtl
			currentMaterial->SetTexture(mtlPath.substr(0, mtlPath.rfind('/')) + "/" + tex);
		}
		else if (params[0] == "kd")
		{
			currentMaterial->SetMaterialColor(glm::vec4(
				atof(params[1].c_str()),
				atof(params[2].c_str()),
				atof(params[3].c_str()),
				1));
		}
		else if (
			params[0] == "ka" ||
			params[0] == "ks" ||
			params[0] == "illum" ||
			params[0] == "map_bump" ||
			params[0] == "map_ke" ||
			params[0] == "map_ka" ||
			params[0] == "map_d" ||
			params[0] == "d" ||
			params[0] == "ke" ||
			params[0] == "ns" ||
			params[0] == "ni" ||
			params[0] == "td" ||
			params[0] == "tf" ||
			params[0] == "tr")
		{
			//Ignored params
		}
		else
			std::cout << "Didn't parse {" << params[0] << "} in material file" << std::endl;
	}

	//Save mtl
	if (currentMaterial != nullptr)
		materials.push_back(currentMaterial);
}

//Replaces a substring in a string
std::string GameObject::Replace(std::string& str, const std::string& toReplace, const std::string& replacement)
{
	size_t index = 0;
	while (true)
	{
		index = str.find(toReplace, index);
		if (index == std::string::npos)
			break;
		str.replace(index, toReplace.length(), replacement);
		++index;
	}
	return str;
}

//Splits a string into substrings, based on a separator
std::vector<std::string> GameObject::Split(std::string str, const std::string& separator)
{
	std::vector<std::string> ret;
	size_t index;
	while (true)
	{
		index = str.find(separator);
		if (index == std::string::npos)
			break;
		ret.push_back(str.substr(0, index));
		str = str.substr(index + 1);
	}
	ret.push_back(str);
	return ret;
}

//Turns a string to lowercase
inline std::string GameObject::ToLower(std::string data)
{
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}

//Cleans up a line for processing
inline std::string GameObject::CleanLine(std::string line)
{
	line = Replace(line, "\t", " ");
	while (line.find("  ") != std::string::npos)
		line = Replace(line, "  ", " ");
	if (line == "")
		return "";
	if (line[0] == ' ')
		line = line.substr(1);
	if (line == "")
		return "";
	if (line[line.length() - 1] == ' ')
		line = line.substr(0, line.length() - 1);
	return line;
}
