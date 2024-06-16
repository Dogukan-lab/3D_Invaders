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
    explicit ObjectLoader(const char* filePath);

    void parseMaterial(const std::string& fileName);
    std::shared_ptr<tigl::VBO> createVBO();

private:
    //Data
    std::vector<glm::vec3> vertices{};
    std::vector<glm::vec2> texCoords{};
    std::vector<glm::vec3> normals{};
    std::vector<Face> faces {};
    std::unordered_map<std::string, Material> materials {};
private:
    //Helper functions to make it more readable.
    void parseVertex(std::istringstream& stringStream);
    void parseTexture(std::istringstream& stringStream);
    void parseNormal(std::istringstream& stringStream);
    void parseFace(std::istringstream& stringStream);
};
