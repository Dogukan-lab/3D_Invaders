#pragma once
#include "../ECS/core/Coordinator.h"
struct GLFWwindow;
/**
* Generic Window setup for Imgui intandem with GLFW.
* This class can be used as an abstract class to make custom GUI's.
*/
class Window {
public:
	void Init(GLFWwindow* window);
	virtual void Update(Coordinator* coordinator, GLFWwindow* window);
	void Render();
	void ShutDown();
private:
	void NewFrame();
};