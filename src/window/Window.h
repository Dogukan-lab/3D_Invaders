#pragma once
struct GLFWwindow;
/**
* Generic Window setup for Imgui intandem with GLFW.
* This class can be used as an abstract class to make custom GUI's.
*/
class Window {
public:
	void Init(GLFWwindow* window);
	virtual void Update();
	void Render();
	void ShutDown();
private:
	void NewFrame();
};