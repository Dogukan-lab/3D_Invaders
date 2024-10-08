//
// Created by doguk on 10-5-2023.
//

#include "FPSCam.h"
#include <GLFW/glfw3.h>

#include <cmath>
#include "glm/gtc/matrix_transform.hpp"

glm::mat4 FPSCam::getMatrix() {
    glm::mat4 ret(1.f);
    ret = glm::rotate(ret, rotation.x, glm::vec3(1, 0, 0));
    ret = glm::rotate(ret, rotation.y, glm::vec3(0, 1, 0));
    ret = glm::rotate(ret, rotation.z, glm::vec3(0, 0, 1));
    ret = glm::translate(ret, position);
    return ret;
}

void FPSCam::update_cam(float deltaTime, bool& isStatic) {
    double x, y;
    if (isStatic)
        return;
    glfwGetCursorPos(this->currentWindow, &x, &y);
    
    static double lastX = x;
    static double lastY = y;

    rotation.x -= (float)(lastY - y) / 100.f;
    rotation.y -= (float)(lastX - x) / 100.f;

    lastX = x;
    lastY = y;


    if (glfwGetKey(this->currentWindow, GLFW_KEY_W))
        move(90, deltaTime);
    if (glfwGetKey(this->currentWindow, GLFW_KEY_S))
        move(-90, deltaTime);
    if (glfwGetKey(this->currentWindow, GLFW_KEY_D))
        move(180, deltaTime);
    if (glfwGetKey(this->currentWindow, GLFW_KEY_A))
        move(0, deltaTime);
    if (glfwGetKey(this->currentWindow, GLFW_KEY_LEFT_SHIFT)) {
        this->position.y += (1 * deltaTime);
    }
    if (glfwGetKey(this->currentWindow, GLFW_KEY_SPACE)) {
        this->position.y -= (1 * deltaTime);
    }
    
}


void FPSCam::move(float angle, float vac) {

    position.x += float(std::cos(rotation.y + glm::radians(angle))) * vac;
    position.z += float(std::sin(rotation.y + glm::radians(angle))) * vac;
}

FPSCam::FPSCam(GLFWwindow* window) {
    this->currentWindow = window;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CENTER_CURSOR);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}
