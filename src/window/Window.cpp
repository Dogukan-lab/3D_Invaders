#include "Window.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../ECS/core/Coordinator.h"
#include "../ECS/core/Transform.h"
#include <GLFW/glfw3.h>
#include <io.h>
#include <optional>
#include <iostream>
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

void EditEntity(std::shared_ptr<Entity> entity) {
    ImGui::Text("Entity ID: %d", entity->entityID);
    auto& transform = entity->getComponent<Transform>();  

    ImGui::Text("Entity: %d, Position", entity->entityID);
    ImGui::SliderFloat(entity->entityID + "  Position X", &transform->position.x, -10, 10);
    ImGui::SliderFloat(entity->entityID + "  Position Y", &transform->position.y, -10, 10);
    ImGui::SliderFloat(entity->entityID + "  Position Z", &transform->position.z, -10, 10);

    ImGui::Text("Entity: %d, Rotation", entity->entityID);
    ImGui::SliderFloat(entity->entityID + "  Rotation X", &transform->rotation.x, -100, 100);
    ImGui::SliderFloat(entity->entityID + "  Rotation Y", &transform->rotation.y, -100, 100);
    ImGui::SliderFloat(entity->entityID + "  Rotation Z", &transform->rotation.z, -100, 100);
}

void Window::Update(Coordinator* coordinator, GLFWwindow* window)
{
    Window::NewFrame();
    //ImGui funstuff.
    ImGui::SetNextWindowSize(ImVec2(400, 600));
    ImGui::Begin("Main Hub", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    //ImGui components before render.

    // ImGui code
    for (const auto& entityPair : coordinator->getEntities()) {
        auto& entity = entityPair.second;
        EditEntity(entity);
        ImGui::Separator();
    }
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
