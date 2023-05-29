#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <../imgui/include/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <iostream>
#include "tigl.h"
#include "camera/fpsCam.h"
#include "ECS/EntityManager.h"
#include "ECS/components/Transform.h"
#include "window/Window.h"

GLFWwindow* glfwWindow;
std::unique_ptr<Window> controlPanel = std::make_unique<Window>();
FPSCam* fpscam;
EntityManager* manager;


tigl::VBO* cubeVBO;

glm::vec3 rotation(0.f);

int viewport[4];

int windowWidth = 640, windowHeight = 480;

void init();
void update();
void draw();

int main(void)
{

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindow = glfwCreateWindow(windowWidth, windowHeight, "Why are you reading this?!??!", NULL, NULL);
    if (!glfwWindow)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(glfwWindow);
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize glad interface!" << std::endl;
        return -1;
    }

    tigl::init();
    init();


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(glfwWindow))
    {
        /* Render here */
        update();
        draw();
        /* Swap front and back buffers */
        glfwSwapBuffers(glfwWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }
    controlPanel->ShutDown();
    glfwTerminate();
    return 0;
}

void init() {
    controlPanel->Init(glfwWindow);
    glfwSetKeyCallback(glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, 1);

        if(key == GLFW_KEY_A)
            rotation.y += 1;

        });

    fpscam = new FPSCam(glfwWindow);
    //manager = new EntityManager();

    //auto entity = manager->addEntity<Entity>();

    //entity->addComponent(Transform());


    std::vector<tigl::Vertex> vertices = {
        //Face of cube GREEN
        tigl::Vertex::PCTN(glm::vec3(-.5f, .5f, -.5f), glm::vec4(0.f, .5f, .0f, 1.f), glm::vec2(0), glm::vec3(0)),
        tigl::Vertex::PCTN(glm::vec3(-.5f, -.5f, -.5f), glm::vec4(0.f, .5f, .0f, 1.f), glm::vec2(0), glm::vec3(0)),
        tigl::Vertex::PCTN(glm::vec3(.5f, -.5f, -.5f), glm::vec4(0.f, .5f, .0f, 1.f), glm::vec2(0), glm::vec3(0)),
        tigl::Vertex::PCTN(glm::vec3(.5f, .5f, -.5f), glm::vec4(0.f, .5f, .0f, 1.f), glm::vec2(0), glm::vec3(0)),
        
        //Left side Cube RED
        tigl::Vertex::PCTN(glm::vec3(-.5f, .5f, -.5f), glm::vec4(.5f, .0f, .0f, 1.f), glm::vec2(0), glm::vec3(0)),
        tigl::Vertex::PCTN(glm::vec3(-.5f, -.5f, -.5f), glm::vec4(.5f, .0f, .0f, 1.f), glm::vec2(0), glm::vec3(0)),
        tigl::Vertex::PCTN(glm::vec3(-.5f, -.5f, .5f), glm::vec4(.5f, .0f, .0f, 1.f), glm::vec2(0), glm::vec3(0)),
        tigl::Vertex::PCTN(glm::vec3(-.5f, .5f, .5f), glm::vec4(.5f, .0f, .0f, 1.f), glm::vec2(0), glm::vec3(0)),

        //Back of cube BLUE
        tigl::Vertex::PCTN(glm::vec3(-.5f, .5f, .5f), glm::vec4(0.f, 0.f, .5f, 1.f), glm::vec2(0), glm::vec3(0)),
        tigl::Vertex::PCTN(glm::vec3(-.5f, -.5f, .5f), glm::vec4(0.f, 0.f, .5f, 1.f), glm::vec2(0), glm::vec3(0)),
        tigl::Vertex::PCTN(glm::vec3(.5f, -.5f, .5f), glm::vec4(0.f, 0.f, .5f, 1.f), glm::vec2(0), glm::vec3(0)),
        tigl::Vertex::PCTN(glm::vec3(.5f, .5f, .5f), glm::vec4(0.f, 0.f, .5f, 1.f), glm::vec2(0), glm::vec3(0)),

        //Right side of cube SOMETHING
        tigl::Vertex::PCTN(glm::vec3(.5f, .5f, -.5f), glm::vec4(0.7f, 0.5f, .1f, 1.f), glm::vec2(0), glm::vec3(0)),
        tigl::Vertex::PCTN(glm::vec3(.5f, -.5f, -.5f), glm::vec4(0.7f, 0.5f, .1f, 1.f), glm::vec2(0), glm::vec3(0)),
        tigl::Vertex::PCTN(glm::vec3(.5f, -.5f, .5f), glm::vec4(0.7f, 0.5f, .1f, 1.f), glm::vec2(0), glm::vec3(0)),
        tigl::Vertex::PCTN(glm::vec3(.5f, .5f, .5f), glm::vec4(0.7f, 0.5f, .1f, 1.f), glm::vec2(0), glm::vec3(0)),

        //Top of cube
        tigl::Vertex::PCTN(glm::vec3(-.5f, .5f, -.5f), glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec2(0), glm::vec3(0)),
        tigl::Vertex::PCTN(glm::vec3(-.5f, .5f, .5f), glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec2(0), glm::vec3(0)),
        tigl::Vertex::PCTN(glm::vec3(.5f, .5f, .5f), glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec2(0), glm::vec3(0)),
        tigl::Vertex::PCTN(glm::vec3(.5f, .5f, -.5f), glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec2(0), glm::vec3(0)),

        //Bottom of cube
        tigl::Vertex::PCTN(glm::vec3(-.5f, -.5f, -.5f), glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec2(0), glm::vec3(0)),
        tigl::Vertex::PCTN(glm::vec3(-.5f, -.5f, .5f), glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec2(0), glm::vec3(0)),
        tigl::Vertex::PCTN(glm::vec3(.5f, -.5f, .5f), glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec2(0), glm::vec3(0)),
        tigl::Vertex::PCTN(glm::vec3(.5f, -.5f, -.5f), glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec2(0), glm::vec3(0)),


    };

    cubeVBO = tigl::createVbo(vertices);
    glEnable(GL_DEPTH_TEST);

}

double lastTime = 0.0;

//TODO Optimaliseer waar nodig is in de cmake :)
//TODO Entity moet een id hebben en kunnen bijhouden wat voor signatuur het heeft qua componenten.
//De reden daarvoor is voornamelijk dat de systems alleen 1 check hoeven te doen ipv constant/per update.
//TODO Maak een mesh component, deze houd voor nu alleen een VBO bij.
//TODO maak een object loader.
//TODO maak de object loader zodanig dat het een vbo altijd terug geeft voor de mesh.
//TODO Textures!
//TODO Belichting uitvogelen!
//TODO cry.
//TODO Maak een eerste systeem waarbij het ECS in z'n geheel wordt getest.
//TODO Maak een eigen Camera systeem :)
void update() {
    controlPanel->Update();
    //Hier ECS dingen testen
    double currentFrame = glfwGetTime();
    float deltaTime = float(currentFrame - lastTime);
    lastTime = currentFrame;

    fpscam->update_cam(deltaTime);

}

void draw() {

    glViewport(0, 0, windowWidth, windowHeight); 
    glGetIntegerv(GL_VIEWPORT, viewport);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);

    tigl::shader->setProjectionMatrix(
        glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.f)
    );
    tigl::shader->setViewMatrix(fpscam->getMatrix());
    tigl::shader->setModelMatrix(glm::mat4(1.0f));

    //glm::mat4 model;

    //model = glm::rotate(model, 10.f, glm::vec3(0, 1, 0));
    //tigl::shader->setModelMatrix(glm::mat4(1.0f));
    tigl::shader->enableColor(true);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    tigl::drawVertices(GL_QUADS, cubeVBO);
    tigl::shader->enableColor(false); 
    controlPanel->Render();
}