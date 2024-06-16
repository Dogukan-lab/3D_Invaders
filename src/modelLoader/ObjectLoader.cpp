//
// Created by doguk on 15-6-2024.
//

#include "ObjectLoader.h"
#include "Material.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <bitset>

const std::string mtlPath("C:/githubrepos/3D_Invaders/resources/spaceship/");
std::string currentMaterial;
std::string currentObj;
int currentSmoothingGroup = 0;

void ObjectLoader::loadObject(const char *filePath, const char* objectName) {
    //Parse object in this
    currentObj = objectName;
    models[currentObj] = nullptr;
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
    createVBO();
    currentObj.clear();
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
    std::ifstream materialFile(mtlPath + fileName);
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
            std::string tempString;
            stringStream >> tempString;
            material.map_Kd = mtlPath + tempString;
//            std::cout << "MAP KD: " << material.map_Kd << "\n";
             
        }
    }

    //After parsing the material
    if (!material.name.empty()) {
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

std::vector<std::string> Split(std::string str, const std::string &separator) {
    std::vector<std::string> ret;
    size_t index;
    while (true) {
        index = str.find(separator);
        if (index == std::string::npos)
            break;
        ret.push_back(str.substr(0, index));
        str = str.substr(index + 1);
    }
    ret.push_back(str);
    return ret;
}

void ObjectLoader::parseFace(std::istringstream &stringStream) {
    std::string faceData{};
    std::vector<std::string> params{};

    while (stringStream >> faceData) {
        params.push_back(faceData);
    }

//    for(const auto& data: params) {
//        std::cout << data << "\t";
//    }
//    std::cout << std::endl;

    for (size_t ii = 3; ii <= params.size(); ii++) {
        Face face;
        for (size_t i = ii - 3; i < ii; i++) {
            std::vector<std::string> indices = Split(params[i == (ii - 3) ? 0 : i], "/");
            if (!indices.empty())
                face.vertexIndices.push_back(std::stoi(indices[0]) - 1);
            if (indices.size() == 2)
                face.texCoordIndices.push_back(std::stoi(indices[1]) - 1);
            if (indices.size() == 3) {
                if (!indices[1].empty())
                    face.texCoordIndices.push_back(std::stoi(indices[1]) - 1);
                face.normalIndices.push_back(std::stoi(indices[2]) - 1);
            }
        }
        this->faces.push_back(face);
    }

//    for(const auto& data : params) {
//        std::cout << "Face data: " <<  data << "\n";
//    }

}


void ObjectLoader::createVBO() {

    std::vector<tigl::Vertex> vbo_vertices;
    for (const auto &face: this->faces) {
        for (int i = 0; i < 3; i++) {
            auto &position = this->vertices[face.vertexIndices[i]];
            auto &normal = this->normals[face.normalIndices[i]];
            auto &texture = this->texCoords[face.texCoordIndices[i]];
            vbo_vertices.push_back(tigl::Vertex::PCTN(position, glm::vec4(1.f), texture, normal));
        }
    }

    std::shared_ptr<tigl::VBO> modelVBO;
    modelVBO.reset(tigl::createVbo(vbo_vertices));
    models[currentObj] = modelVBO;
    vbo_vertices.clear();
}

void ObjectLoader::printMaterials() {
    std::ostringstream stringStream;
    for(const auto& matPair: materials) {
        auto& mat = matPair.second;
        stringStream << mat;
        std::cout << stringStream.str() << "\n";
    }
}

Material& ObjectLoader::getMaterial(const std::string& mat) {
    return materials[mat];
}

