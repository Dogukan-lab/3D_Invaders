#include "Window.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <io.h>

//TODO make the coordinator useful in the imgui UI!
//extern Coordinator gcoordinator; Use this to get access to entitymanager and do fun stuff with it :)

//TODO Make it so that the entity Manager can pass list to the window
void Window::Init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

}

void Window::NewFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Window::Update()
{
    Window::NewFrame();
    static float value = 0.f;
    //ImGui funstuff.
    ImGui::Begin("Main Hub");
    //ImGui components before render.
    ImGui::Text("This is a test to check if it works!");
    ImGui::SliderFloat("Float thingy", &value, 0.f, 1.f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void Window::Render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::ShutDown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
