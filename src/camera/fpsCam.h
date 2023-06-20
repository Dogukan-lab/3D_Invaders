//
// Created by doguk on 10-5-2023.
//
#pragma once
#include "glm/glm.hpp"
struct GLFWwindow;

class FPSCam {
public:
    FPSCam(GLFWwindow* window);

    glm::mat4 getMatrix();
    void update_cam(float deltaTime, bool& isStatic);

    inline void setPosition(const glm::vec3& newPos) {
        this->position = newPos;
    }

private:
    glm::vec3 position = glm::vec3(0.f);
    glm::vec2 rotation = glm::vec2(0.f);
    void move(float angle, float vac);

    GLFWwindow* currentWindow;
}; 