#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <../imgui/include/imgui_impl_opengl3.h>
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include "tigl.h"
#include "camera/fpsCam.h"
#include "window/Window.h"
#include "ECS/core/Mesh.h"
#include "ECS/core/Transform.h"
#include "ECS/core/Coordinator.h"
#include "ECS/systems/RenderSystem.h"
#include "modelLoader/ModelLoader.h"

GLFWwindow* glfwWindow;
std::unique_ptr<Window> controlPanel = std::make_unique<Window>();
FPSCam* fpscam;
Coordinator* ecsCoordinator;
ModelLoader* modelLoader;

//struct RenderSystem;

tigl::VBO* cubeVBO;

glm::vec3 rotation(0.f);

int viewport[4];

int windowWidth = 1280, windowHeight = 1080;

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

    glfwSetWindowMonitor(glfwWindow, NULL, 0, 0, windowWidth, windowHeight, 60);

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

void printEntity(std::shared_ptr<Entity> entity) {
    if (!entity)
        return;
    std::cout << "Entity ID: " << entity->entityID << std::endl;

   /* auto transform = entity->getComponent<Transform>();
    if (!transform)
        return;*/

    std::cout << "Entity Signature: " << entity->getSig().to_string() << std::endl;
    
    //Transform debug code
    /*std::cout << "Transform Position: " << glm::to_string(transform->position) << std::endl;
    std::cout << "Transform Rotation: " << glm::to_string(transform->rotation) << std::endl;
    std::cout << "Transform Scale: " << glm::to_string(transform->scale) << std::endl;
    std::cout << "Component entity ref ID: " << transform->entityRef->entityID << std::endl;*/
    std::cout << "Entity ID from Mesh: " << entity->getComponent<Mesh>()->entityRef->entityID << std::endl;

    std::cout << "\n";
}

void stressTest(tigl::VBO* vbo) {
    for (int i = 0; i < types::MAX_ENTITIES; i++) {
        auto entity = ecsCoordinator->createEntity();
    }

    ecsCoordinator->getEntity(2)->addComponent<Mesh>()->setMesh(vbo);
    printEntity(ecsCoordinator->getEntity(2));
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
    modelLoader = new ModelLoader();
    ecsCoordinator = new Coordinator();

    
    std::vector<tigl::Vertex> quadVertices = {  
        //Face of cube GREEN
        tigl::Vertex::PCTN(glm::vec3(-.5f, .5f, -.5f), glm::vec4(0.f, .5f, .0f, 1.f), glm::vec2(0), glm::vec3(0, 0, 1)),
        tigl::Vertex::PCTN(glm::vec3(-.5f, -.5f, -.5f), glm::vec4(0.f, .5f, .0f, 1.f), glm::vec2(0), glm::vec3(0, 0, 1)),
        tigl::Vertex::PCTN(glm::vec3(.5f, -.5f, -.5f), glm::vec4(0.f, .5f, .0f, 1.f), glm::vec2(0), glm::vec3(0, 0, 1)),
        tigl::Vertex::PCTN(glm::vec3(.5f, .5f, -.5f), glm::vec4(0.f, .5f, .0f, 1.f), glm::vec2(0), glm::vec3(0, 0, 1)),
        
        //Left side Cube RED
        tigl::Vertex::PCTN(glm::vec3(-.5f, .5f, -.5f), glm::vec4(.5f, .0f, .0f, 1.f), glm::vec2(0), glm::vec3(-1, 0, 0)),
        tigl::Vertex::PCTN(glm::vec3(-.5f, -.5f, -.5f), glm::vec4(.5f, .0f, .0f, 1.f), glm::vec2(0), glm::vec3(-1, 0, 0)),
        tigl::Vertex::PCTN(glm::vec3(-.5f, -.5f, .5f), glm::vec4(.5f, .0f, .0f, 1.f), glm::vec2(0), glm::vec3(-1, 0, 0)),
        tigl::Vertex::PCTN(glm::vec3(-.5f, .5f, .5f), glm::vec4(.5f, .0f, .0f, 1.f), glm::vec2(0), glm::vec3(-1, 0, 0)),

        //Back of cube BLUE
        tigl::Vertex::PCTN(glm::vec3(-.5f, .5f, .5f), glm::vec4(0.f, 0.f, .5f, 1.f), glm::vec2(0), glm::vec3(0, 0, -1)),
        tigl::Vertex::PCTN(glm::vec3(-.5f, -.5f, .5f), glm::vec4(0.f, 0.f, .5f, 1.f), glm::vec2(0), glm::vec3(0, 0, -1)),
        tigl::Vertex::PCTN(glm::vec3(.5f, -.5f, .5f), glm::vec4(0.f, 0.f, .5f, 1.f), glm::vec2(0), glm::vec3(0, 0, -1)),
        tigl::Vertex::PCTN(glm::vec3(.5f, .5f, .5f), glm::vec4(0.f, 0.f, .5f, 1.f), glm::vec2(0), glm::vec3(0, 0, -1)),

        //Right side of cube SOMETHING
        tigl::Vertex::PCTN(glm::vec3(.5f, .5f, -.5f), glm::vec4(0.7f, 0.5f, .1f, 1.f), glm::vec2(0), glm::vec3(1, 0, 0)),
        tigl::Vertex::PCTN(glm::vec3(.5f, -.5f, -.5f), glm::vec4(0.7f, 0.5f, .1f, 1.f), glm::vec2(0), glm::vec3(1, 0, 0)),
        tigl::Vertex::PCTN(glm::vec3(.5f, -.5f, .5f), glm::vec4(0.7f, 0.5f, .1f, 1.f), glm::vec2(0), glm::vec3(1, 0, 0)),
        tigl::Vertex::PCTN(glm::vec3(.5f, .5f, .5f), glm::vec4(0.7f, 0.5f, .1f, 1.f), glm::vec2(0), glm::vec3(1, 0, 0)),

        //Top of cube
        tigl::Vertex::PCTN(glm::vec3(-.5f, .5f, -.5f), glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec2(0), glm::vec3(0, 1, 0)),
        tigl::Vertex::PCTN(glm::vec3(-.5f, .5f, .5f), glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec2(0), glm::vec3(0, 1, 0)),
        tigl::Vertex::PCTN(glm::vec3(.5f, .5f, .5f), glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec2(0), glm::vec3(0, 1, 0)),
        tigl::Vertex::PCTN(glm::vec3(.5f, .5f, -.5f), glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec2(0), glm::vec3(0, 1, 0)),

        //Bottom of cube
        tigl::Vertex::PCTN(glm::vec3(-.5f, -.5f, -.5f), glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec2(0), glm::vec3(0, -1, 0)),
        tigl::Vertex::PCTN(glm::vec3(-.5f, -.5f, .5f), glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec2(0), glm::vec3(0, -1, 0)),
        tigl::Vertex::PCTN(glm::vec3(.5f, -.5f, .5f), glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec2(0), glm::vec3(0, -1, 0)),
        tigl::Vertex::PCTN(glm::vec3(.5f, -.5f, -.5f), glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec2(0), glm::vec3(0, -1, 0)),


    };

    std::vector<tigl::Vertex> triangleVertices {

    };

    cubeVBO = tigl::createVbo(quadVertices);

    //stressTest(cubeVBO);
    modelLoader->loadModel("D:\\githubrepos\\3D_Invaders\\resources\\models\\suzanne.obj");
    ecsCoordinator->registerSystem<RenderSystem>();
    auto& entity = ecsCoordinator->createEntity();
    ecsCoordinator->addComponent<Mesh>(entity->entityID)->drawable = modelLoader->createVBO(); //Deze lijkt redundant als ik toch al entity backref xD
    ecsCoordinator->addComponent<Transform>(entity->entityID); //Deze lijkt redundant als ik toch al entity backref xD
    
    auto& entity2 = ecsCoordinator->createEntity();
    ecsCoordinator->addComponent<Mesh>(entity2->entityID)->drawable = modelLoader->createVBO();
    ecsCoordinator->addComponent<Transform>(entity2->entityID)->position = {-5, 2, 3};

    modelLoader->loadModel("D:\\githubrepos\\3D_Invaders\\resources\\models\\chr_knight.obj");
    auto& entity3 = ecsCoordinator->createEntity();
    ecsCoordinator->addComponent<Mesh>(entity3->entityID)->drawable = modelLoader->createVBO();
    ecsCoordinator->addComponent<Transform>(entity3->entityID)->position = { 5, -1, 4 };

    auto& entity4 = ecsCoordinator->createEntity();
    //ecsCoordinator->addComponent<Mesh>(entity4->entityID)->drawable = worldPlane;
    ecsCoordinator->addComponent<Transform>(entity4->entityID);
    glEnable(GL_DEPTH_TEST);
}

double lastTime = 0.0;

//TODO maak een object loader.
//TODO maak de object loader zodanig dat het een vbo altijd terug geeft voor de mesh.
//TODO Textures!
//TODO Belichting uitvogelen!
//TODO cry.
//TODO Maak een eerste systeem waarbij het ECS in z'n geheel wordt getest.
//TODO Maak een eigen Camera systeem :)
void update() {   
    controlPanel->Update(ecsCoordinator);
    //Hier ECS dingen testen

    double currentFrame = glfwGetTime();
    float deltaTime = float(currentFrame - lastTime);
    lastTime = currentFrame;

    //ecsCoordinator->getEntity(0)->getComponent<Transform>()->position.x += (1 * deltaTime);
    ecsCoordinator->getEntity(0)->getComponent<Transform>()->rotation.y += (1 * deltaTime);
    ecsCoordinator->getEntity(1)->getComponent<Transform>()->rotation.x += (1 * deltaTime);
    ecsCoordinator->getEntity(2)->getComponent<Transform>()->rotation.z += (1 * deltaTime);
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

    //tigl::shader->enableColor(true);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ecsCoordinator->getSystem<RenderSystem>()->draw();
    controlPanel->Render();
}