//
// Created by doguk on 15-6-2024.
//

#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "tigl.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "Material.h"

struct Face {
    std::vector<int> vertexIndices{};
    std::vector<int> texCoordIndices{};
    std::vector<int> normalIndices{};
    std::string materialName{};
    int smoothingGroup;
};

struct IndexedVertex {
    int vertexIndex;
    int texIndex;
    int normalIndex;
//    std::string materialName{};
//    int smoothingGroup;
};
class ObjectLoader {
public:
    ObjectLoader() = default;

    void loadObject(const char* filePath, const char* objectName);
    void parseMaterial(const std::string& fileName);
    void createVBO();

    void printMaterials();

    inline std::weak_ptr<tigl::VBO> getVBO(const char* name) {
        return models[name];
    }

    Material& getMaterial(const std::string& mat);

private:
    //Data
    std::vector<glm::vec3> vertices{};
    std::vector<glm::vec2> texCoords{};
    std::vector<glm::vec3> normals{};
    std::vector<Face> faces {};
    std::unordered_map<std::string, Material> materials {};
    std::unordered_map<std::string, std::shared_ptr<tigl::VBO>> models{};
private:
    //Helper functions to make it more readable.
    void parseVertex(std::istringstream& stringStream);
    void parseTexture(std::istringstream& stringStream);
    void parseNormal(std::istringstream& stringStream);
    void parseFace(std::istringstream& stringStream);
};
