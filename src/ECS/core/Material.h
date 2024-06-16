//
// Created by doguk on 16-6-2024.
//
#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <ostream>

struct Material {
    std::string name;
    glm::vec3 ambient; //Ka
    glm::vec3 diffuse; //Kd
    glm::vec3 specular; //Ks
    float specExponent; //Ns
    float transparency; //d
    int illumination; //illum
    std::string map_Kd; //Diffuse texture map

    friend std::ostream &operator<<(std::ostream &os, const Material &mat) {
        os << "Mat name: " << mat.name << "\n";
        os << "Mat ambient: " << glm::to_string(mat.ambient) << "\n";
        os << "Mat diffuse: " << glm::to_string(mat.diffuse) << "\n";
        os << "Mat specular: " << glm::to_string(mat.specular) << "\n";
        os << "Mat spec exponent: " << mat.specExponent << "\n";
        os << "Mat transparency: " << mat.transparency << "\n";
        os << "Mat illumination: " << mat.illumination << "\n";
        os << "Mat diffuse_map: " << mat.map_Kd << "\n";
        return os;
    }
};