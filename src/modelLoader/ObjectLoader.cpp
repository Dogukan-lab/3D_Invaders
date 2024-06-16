//
// Created by doguk on 15-6-2024.
//

#include "ObjectLoader.h"
#include "Material.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string currentMaterial;
int currentSmoothingGroup = 0;

ObjectLoader::ObjectLoader(const char *filePath) {
    //Parse object in this

    std::ifstream file(filePath);
    if (!file.is_open())
        std::cerr << "Couldn't open or find file!" << std::endl;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stringStream(line);
        std::string type;
        stringStream >> type;

        if (type == "v") {
            //Parse vertex data
            parseVertex(stringStream);
        } else if (type == "vt") {
            //Parse vertex texture data
            parseTexture(stringStream);
        } else if (type == "vn") {
            //Parse Vertex normal data
            parseNormal(stringStream);
        } else if (type == "f") {
            parseFace(stringStream);
            //Parse face data
        } else if (type == "mtllib") {
            //Set material file
            std::string materialFile;
            stringStream >> materialFile;
            parseMaterial(materialFile);
        } else if (type == "usemtl") {
            //Set current material
            stringStream >> currentMaterial;
        } else if (type == "s") {
            //Set smoothing for face
            stringStream >> currentSmoothingGroup;
        } else if (type == "g") {
            //Set group.
            //Do nothing for first rendition of object loader.
        }
    }
    file.close();
}

void ObjectLoader::parseVertex(std::istringstream &stringStream) {
    glm::vec3 vertex;
    stringStream >> vertex.x >> vertex.y >> vertex.z;
    vertices.push_back(vertex);
}

void ObjectLoader::parseTexture(std::istringstream &stringStream) {
    glm::vec2 texture;
    stringStream >> texture.x >> texture.y;
    texCoords.push_back(texture);
}

void ObjectLoader::parseNormal(std::istringstream &stringStream) {
    glm::vec3 normal;
    stringStream >> normal.x >> normal.y >> normal.z;
    normals.push_back(normal);
}

void ObjectLoader::parseMaterial(const std::string &fileName) {
    std::ifstream materialFile("../resources/spaceship/" + fileName);
    if (!materialFile.is_open())
        std::cerr << "File could not be opened!" << std::endl;

    std::string line;
    Material material;
    while (std::getline(materialFile, line)) {
        std::istringstream stringStream(line);
        std::string type;
        stringStream >> type;
        if (type == "newmtl") {
            if (!material.name.empty()) {
                materials[material.name] = material;
            }
            material = Material();
            stringStream >> material.name;
        } else if (type == "Ka") {
            stringStream >> material.ambient.x >> material.ambient.y >> material.ambient.z;
        } else if (type == "Kd") {
            stringStream >> material.diffuse.x >> material.diffuse.y >> material.diffuse.z;
        } else if (type == "Ks") {
            stringStream >> material.specular.x >> material.specular.y >> material.specular.z;
        } else if (type == "Ns") {
            stringStream >> material.specExponent;
        } else if (type == "d") {
            stringStream >> material.transparency;
        } else if (type == "illum") {
            stringStream >> material.illumination;
        } else if (type == "map_Kd") {
            stringStream >> material.map_Kd;
        }
    }

    //After parsing the material
    if(!material.name.empty()) {
        materials[material.name] = material;
    }
    //Debug code for reading the parsed material.
//    for(const auto& matpair : materials) {
//        std::stringstream stringStream;
//        const auto& mat = matpair.second;
//        stringStream << mat;
//        std::cout << stringStream.str();
//    }
    materialFile.close();
}

void ObjectLoader::parseFace(std::istringstream &stringStream) {
    Face face;
    std::string vertexString;
    while (stringStream >> vertexString) {
        std::stringstream viss(vertexString);
        std::string index;
        int indices[3] = {0, 0, 0};
        int i = 0;
        //Parse line per face and use '/' as delimeter.
        while (std::getline(viss, index, '/') && i < 3) {
            if (!index.empty()) {
                indices[i] = std::stoi(index) - 1;
            }
            i++;
        }
        //Add indices to face indices
        face.vertexIndices.push_back(index[0]);
        //Check if there's only vertices
        if (i > 1) face.texCoordIndices.push_back(index[1]);
        if (i > 2) face.normalIndices.push_back(index[3]);
    }
}

std::shared_ptr<tigl::VBO> ObjectLoader::createVBO() {
//    Mesh -> material
//    Mesh -> drawable (VBO)
    std::vector<tigl::Vertex> vbo_vertices;
    for(const auto& face: this->faces) {
        for(int i = 0; i < 3; i++) {
            auto& position = this->vertices[face.vertexIndices[i]];
            auto& normal = this->normals[face.normalIndices[i]];
            vbo_vertices.push_back(tigl::Vertex::PCN(position, glm::vec4(1.f), normal));
        }
    }
    std::shared_ptr<tigl::VBO> modelVBO;
    modelVBO.reset(tigl::createVbo(vbo_vertices));
    vbo_vertices.clear();
    return modelVBO;
}
