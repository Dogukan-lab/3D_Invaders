#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <../imgui/include/imgui_impl_opengl3.h>
#include <iostream>
#include <sstream>
#include "glm/gtx/string_cast.hpp"
#include "tigl.h"
#include "camera/fpsCam.h"
#include "window/Window.h"
#include "ECS/core/Mesh.h"
#include "ECS/core/Transform.h"
#include "ECS/core/Coordinator.h"
#include "ECS/core/LightComponent.h"
#include "ECS/core/TextureComponent.h"
#include "ECS/systems/RenderSystem.h"
#include "modelLoader/ModelLoader.h"
#include "UnitTester.h"
#include "modelLoader/ObjectLoader.h"

#define TESTING 0
#define OBJLOADER 0

//TODO Refactor these raw pointers!
GLFWwindow *glfwWindow;
std::unique_ptr<Window> controlPanel = std::make_unique<Window>();
std::unique_ptr<FPSCam> fpscam;
std::shared_ptr<Coordinator> ecsCoordinator;
std::unique_ptr<ObjectLoader> objLoader;

std::shared_ptr<tigl::VBO> vbo;
std::shared_ptr<tigl::VBO> worldPlane;

glm::vec3 rotation(0.f);

int viewport[4];

int windowWidth = 1280, windowHeight = 1080;
constexpr float tileSize = 32.f;

void init();

void update();

void draw();

void decompose();

int main() {
#if TESTING
    return UnitTester::startTests();
#endif
#if OBJLOADER
    testObjLoader();
    return 0;
#endif

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindow = glfwCreateWindow(windowWidth, windowHeight, "Why are you reading this", nullptr, nullptr);
    if (!glfwWindow) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(glfwWindow);
    if (!gladLoadGL((GLADloadfunc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize glad interface!" << std::endl;
        return -1;
    }

    tigl::init();
    init();
    glfwSwapInterval(1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(glfwWindow)) {
        /* Render here */
        update();
        draw();
        /* Swap front and back buffers */
        glfwSwapBuffers(glfwWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

//    decompose();
    return 0;


}

void decompose() {
    controlPanel->ShutDown();
    glfwTerminate();
}

void setupTerrain() {
    //Set world plane
    std::vector<tigl::Vertex> triangleVertices{
            tigl::Vertex::PT({-.5f, -.5f, .5f}, glm::vec2(0 * tileSize, 0 * tileSize)),
            tigl::Vertex::PT({-.5f, -.5f, -.5f}, glm::vec2(0 * tileSize, 0 * tileSize)),
            tigl::Vertex::PT({.5f, -.5f, .5f}, glm::vec2(1 * tileSize, 0 * tileSize)),

            tigl::Vertex::PT({.5f, -.5f, .5f}, glm::vec2(1 * tileSize, 0 * tileSize)),
            tigl::Vertex::PT({.5f, -.5f, -.5f}, glm::vec2(1 * tileSize, 0 * tileSize)),
            tigl::Vertex::PT({-.5f, -.5f, -.5f}, glm::vec2(0 * tileSize, 0 * tileSize)),
    };

    worldPlane.reset(tigl::createVbo(triangleVertices));
    const auto &planeEntity = ecsCoordinator->createEntity();
    const auto& planeMesh = ecsCoordinator->addComponent<Mesh>(planeEntity);
    planeMesh->drawable = worldPlane;
    planeMesh->colour = glm::vec3(1.f);
    ecsCoordinator->addComponent<Transform>(planeEntity)->position = glm::vec3(0.f,0.f,0.f);

    //Sun
    const auto& sunEntity = ecsCoordinator->createEntity();
    const auto& sunTransform = ecsCoordinator->addComponent<Transform>(sunEntity);
    const auto& sunMesh = ecsCoordinator->addComponent<Mesh>(sunEntity);
    const auto& sunTexture = ecsCoordinator->addComponent<TextureComponent>(sunEntity);
    const auto& sunLight = ecsCoordinator->addComponent<LightComponent>(sunEntity);

    sunTransform->position = glm::vec3(0.f);
    sunTransform->scale = glm::vec3(1.f);
    sunMesh->drawable = objLoader->getVBO("sun").lock();
    sunTexture->loadTexture(objLoader->getMaterial("moon").map_Kd);
    sunLight->position = sunTransform->position;
    tigl::shader->setLightDirectional(0, true);

    //Asteroid(s)
    const auto& asteroidEntity1 = ecsCoordinator->createEntity();
    const auto& asteroidTransform1 = ecsCoordinator->addComponent<Transform>(asteroidEntity1);
    const auto& asteroidMesh1 = ecsCoordinator->addComponent<Mesh>(asteroidEntity1);
    const auto& asteroidTexture1 = ecsCoordinator->addComponent<TextureComponent>(asteroidEntity1);
    asteroidTransform1->position = sunTransform->position + glm::vec3(0,0, -2);
    asteroidTransform1->scale = glm::vec3(0.1f);
    asteroidMesh1->drawable = objLoader->getVBO("asteroid").lock();
    asteroidTexture1->loadTexture(objLoader->getMaterial("asteroid_mat").map_Kd);

    const auto& asteroidEntity2 = ecsCoordinator->createEntity();
    const auto& asteroidTransform2 = ecsCoordinator->addComponent<Transform>(asteroidEntity2);
    const auto& asteroidMesh2 = ecsCoordinator->addComponent<Mesh>(asteroidEntity2);
    const auto& asteroidTexture2 = ecsCoordinator->addComponent<TextureComponent>(asteroidEntity2);
    asteroidTransform2->position = sunTransform->position + glm::vec3(0,0, -2);
    asteroidTransform2->scale = glm::vec3(0.1f);
    asteroidMesh2->drawable = objLoader->getVBO("asteroid").lock();
    asteroidTexture2->loadTexture(objLoader->getMaterial("asteroid_mat").map_Kd);

    const auto& asteroidEntity3 = ecsCoordinator->createEntity();
    const auto& asteroidTransform3 = ecsCoordinator->addComponent<Transform>(asteroidEntity3);
    const auto& asteroidMesh3 = ecsCoordinator->addComponent<Mesh>(asteroidEntity3);
    const auto& asteroidTexture3 = ecsCoordinator->addComponent<TextureComponent>(asteroidEntity3);
    asteroidTransform3->position = sunTransform->position + glm::vec3(0,0, -2);
    asteroidTransform3->scale = glm::vec3(0.1f);
    asteroidMesh3->drawable = objLoader->getVBO("asteroid").lock();
    asteroidTexture3->loadTexture(objLoader->getMaterial("asteroid_mat").map_Kd);
}

void setupShip(const std::weak_ptr<tigl::VBO> &vbo) {
    const auto &shipEntity = ecsCoordinator->createEntity();
    //Setup ship stuff
    const auto &transform = ecsCoordinator->addComponent<Transform>(shipEntity);
    transform->scale = glm::vec3(0.1f);
    transform->rotation = fpscam->getRotation();
    const auto &mesh = ecsCoordinator->addComponent<Mesh>(shipEntity);
    mesh->drawable = vbo.lock();
//    mesh->colour = glm::vec3(0, 0, 1.f);
//    ecsCoordinator->addComponent<LightComponent>(shipEntity)->position = fpscam->getPos();
    const auto& textureComp = ecsCoordinator->addComponent<TextureComponent>(shipEntity);
    textureComp->loadTexture(objLoader->getMaterial("spaceship").map_Kd);
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetKeyCallback(glfwWindow, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, 1);
    });
//Shader settings
    tigl::shader->enableLighting(true);
    tigl::shader->setLightCount(1);
    tigl::shader->setShinyness(10.f);

    //Camera Light
    tigl::shader->setLightDirectional(0, false);
    tigl::shader->setLightAmbient(0, glm::vec3(0.25f, 0.25f, 0.25f));
    tigl::shader->setLightDiffuse(0, glm::vec3(0.9f, 0.9f, 0.9f));

    controlPanel->Init(glfwWindow);
    ecsCoordinator = std::make_unique<Coordinator>();
    ecsCoordinator->registerSystem<RenderSystem>();
    fpscam = std::make_unique<FPSCam>(glfwWindow);
    fpscam->setPosition(glm::vec3(0.f));
    objLoader = std::make_unique<ObjectLoader>();
    objLoader->loadObject("../resources/spaceship/lowpoly_spaceship.obj", "ship");
    objLoader->loadObject("../resources/spaceship/asteroid.obj", "asteroid");
    objLoader->loadObject("../resources/spaceship/sun.obj", "sun");
    setupShip(objLoader->getVBO("ship"));
    setupTerrain();
}


void printEntity(const std::weak_ptr<Entity> &entity) {
    if (entity.expired())
        return;
    std::cout << "Entity ID: " << entity.lock()->entityID << std::endl;

    /* auto transform = entity->getComponent<Transform>();
     if (!transform)
         return;*/

    std::cout << "Entity Signature: " << entity.lock()->getSig().to_string() << std::endl;

    //Transform debug code
//    std::cout << "Transform Position: " << glm::to_string(transform->position) << std::endl;
//    std::cout << "Transform Rotation: " << glm::to_string(transform->rotation) << std::endl;
//    std::cout << "Transform Scale: " << glm::to_string(transform->scale) << std::endl;
//    std::cout << "Component entity ref ID: " << transform->entityRef->entityID << std::endl;
//    std::cout << "Entity ID from Mesh: " << entity->getComponent<Mesh>() << std::endl;

    std::cout << "\n";
}

void stressTest(std::shared_ptr<tigl::VBO> vbo) {
    for (int i = 0; i < types::MAX_ENTITIES; i++) {
        auto entity = ecsCoordinator->createEntity();
    }

    ecsCoordinator->getEntity(2)->addComponent<Mesh>()->setMesh(vbo);
    printEntity(ecsCoordinator->getEntity(2));
}


double lastTime = 0.0;
double applicationTime;
constexpr double asteroidFlyingRadius = 2.f;
constexpr double timeStep = 0.005;
bool frameIsStatic = false;



//TODO Maak een eigen Camera systeem :)
void update() {
    double currentFrame = glfwGetTime();
    auto deltaTime = float(currentFrame - lastTime);
    lastTime = currentFrame;
    applicationTime += timeStep;


    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_ALT)) {
        frameIsStatic = !frameIsStatic;
    }

    fpscam->update_cam(deltaTime, frameIsStatic);
    controlPanel->Update(ecsCoordinator, glfwWindow);

    //Ship movement.
    const auto &ship = ecsCoordinator->getEntity(0);
    const auto &shipTransform = ship->getComponent<Transform>();
    glm::vec3 point = glm::vec3(0, -0.25f, -0.6f);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.f), fpscam->getRotation().y, glm::vec3(0, 1, 0));
    shipTransform->rotation = (-fpscam->getRotation() + glm::vec3(0, glm::radians(180.f), 0)) * glm::vec3(0, 1, 0);
    shipTransform->position = -fpscam->getPos() + glm::vec3(glm::vec4(point, 1.f) * rotationMatrix);

    //Sun position
    const auto& sun = ecsCoordinator->getEntity(2);
    const auto& sunTransform = sun->getComponent<Transform>();
    const auto& light = sun->getComponent<LightComponent>();
    float distance = glm::distance(shipTransform->position, sunTransform->position);
    light->position = glm::vec3(distance/2.f);

    //Asteroid ordnance.
    const auto& asteroid1 = ecsCoordinator->getEntity(3);
    const auto& asteroid1Transform = asteroid1->getComponent<Transform>();
    asteroid1Transform->position = glm::vec3(glm::cos(applicationTime)*asteroidFlyingRadius, 0, sin(applicationTime)*asteroidFlyingRadius);
    //Asteroid ordnance.
    const auto& asteroid2 = ecsCoordinator->getEntity(4);
    const auto& asteroid2Transform = asteroid2->getComponent<Transform>();
    asteroid2Transform->position = glm::vec3(glm::cos(applicationTime + 3.f)*asteroidFlyingRadius, 0, sin(applicationTime + 3.f)*asteroidFlyingRadius);
    //Asteroid ordnance.
    const auto& asteroid3 = ecsCoordinator->getEntity(5);
    const auto& asteroid3Transform = asteroid3->getComponent<Transform>();
    asteroid3Transform->position = glm::vec3(glm::cos(applicationTime + 5.f)*asteroidFlyingRadius, 0, sin(applicationTime + 5.f)*asteroidFlyingRadius);
}


void draw() {
    glViewport(0, 0, windowWidth, windowHeight);
    glGetIntegerv(GL_VIEWPORT, viewport);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.05f, 0.1f, 0.2f, 1.0f);

    //Camera setup before drawing.
    tigl::shader->setProjectionMatrix(
            glm::perspective(glm::radians(75.0f), viewport[2] / (float) viewport[3], 0.01f, 100.f)
    );
    tigl::shader->setViewMatrix(fpscam->getMatrix());
    tigl::shader->setModelMatrix(glm::mat4(1.0f));

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ecsCoordinator->getSystem<RenderSystem>()->draw();
    controlPanel->Render();
}