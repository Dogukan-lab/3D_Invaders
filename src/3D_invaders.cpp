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
#include "UnitTester.h"
#include "modelLoader/ObjectLoader.h"

#define TESTING 0

//TODO Refactor these raw pointers!
GLFWwindow *glfwWindow;
std::unique_ptr<Window> controlPanel = std::make_unique<Window>();
std::unique_ptr<FPSCam> fpscam;
std::shared_ptr<Coordinator> ecsCoordinator;
std::unique_ptr<ObjectLoader> objLoader;

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

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindow = glfwCreateWindow(windowWidth, windowHeight, "3D Invaders", nullptr, nullptr);
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

    decompose();
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
    const auto &planeMesh = ecsCoordinator->addComponent<Mesh>(planeEntity);
    planeMesh->drawable = worldPlane;
    planeMesh->colour = glm::vec3(1.f);
    ecsCoordinator->addComponent<Transform>(planeEntity)->position = glm::vec3(0.f, 0.f, 0.f);

    //Sun
    const auto &sunEntity = ecsCoordinator->createEntity();
    const auto &sunTransform = ecsCoordinator->addComponent<Transform>(sunEntity);
    const auto &sunMesh = ecsCoordinator->addComponent<Mesh>(sunEntity);
    const auto &sunTexture = ecsCoordinator->addComponent<TextureComponent>(sunEntity);
    const auto &sunLight = ecsCoordinator->addComponent<LightComponent>(sunEntity);
    sunTransform->position = glm::vec3(0.f);
    sunTransform->scale = glm::vec3(1.f);
    sunMesh->drawable = objLoader->getVBO("sun").lock();
    sunTexture->loadTexture(objLoader->getMaterial("sun").map_Kd);
    sunLight->position = sunTransform->position;
    tigl::shader->setLightDirectional(0, true);

    //Asteroid(s)
    const auto &asteroidEntity1 = ecsCoordinator->createEntity();
    const auto &asteroidTransform1 = ecsCoordinator->addComponent<Transform>(asteroidEntity1);
    const auto &asteroidMesh1 = ecsCoordinator->addComponent<Mesh>(asteroidEntity1);
    const auto &asteroidTexture1 = ecsCoordinator->addComponent<TextureComponent>(asteroidEntity1);
    asteroidTransform1->position = sunTransform->position + glm::vec3(0, 0, -2);
    asteroidTransform1->scale = glm::vec3(0.1f);
    asteroidMesh1->drawable = objLoader->getVBO("asteroid").lock();
    asteroidTexture1->loadTexture(objLoader->getMaterial("asteroid_mat").map_Kd);

    const auto &asteroidEntity2 = ecsCoordinator->createEntity();
    const auto &asteroidTransform2 = ecsCoordinator->addComponent<Transform>(asteroidEntity2);
    const auto &asteroidMesh2 = ecsCoordinator->addComponent<Mesh>(asteroidEntity2);
    const auto &asteroidTexture2 = ecsCoordinator->addComponent<TextureComponent>(asteroidEntity2);
    asteroidTransform2->position = sunTransform->position + glm::vec3(0, 0, -2);
    asteroidTransform2->scale = glm::vec3(0.1f);
    asteroidMesh2->drawable = objLoader->getVBO("asteroid").lock();
    asteroidTexture2->loadTexture(objLoader->getMaterial("asteroid_mat").map_Kd);

    const auto &asteroidEntity3 = ecsCoordinator->createEntity();
    const auto &asteroidTransform3 = ecsCoordinator->addComponent<Transform>(asteroidEntity3);
    const auto &asteroidMesh3 = ecsCoordinator->addComponent<Mesh>(asteroidEntity3);
    const auto &asteroidTexture3 = ecsCoordinator->addComponent<TextureComponent>(asteroidEntity3);
    asteroidTransform3->position = sunTransform->position + glm::vec3(0, 0, -2);
    asteroidTransform3->scale = glm::vec3(0.1f);
    asteroidMesh3->drawable = objLoader->getVBO("asteroid").lock();
    asteroidTexture3->loadTexture(objLoader->getMaterial("asteroid_mat").map_Kd);

    //Neptunus
    const auto &neptuneEntity = ecsCoordinator->createEntity();
    const auto &neptuneTransform = ecsCoordinator->addComponent<Transform>(neptuneEntity);
    const auto &neptuneMesh = ecsCoordinator->addComponent<Mesh>(neptuneEntity);
    const auto &neptuneTexture = ecsCoordinator->addComponent<TextureComponent>(neptuneEntity);
    neptuneTransform->position = glm::vec3(-2.f, 2.f, 8.f);
    neptuneMesh->drawable = objLoader->getVBO("neptunus").lock();
    neptuneTexture->loadTexture(objLoader->getMaterial("neptune").map_Kd);

    //Mars
    const auto &marsEntity = ecsCoordinator->createEntity();
    const auto &marsTransform = ecsCoordinator->addComponent<Transform>(marsEntity);
    const auto &marsMesh = ecsCoordinator->addComponent<Mesh>(marsEntity);
    const auto &marsTexture = ecsCoordinator->addComponent<TextureComponent>(marsEntity);
    marsTransform->position = glm::vec3(-5.f, -3.f, 15.f);
    marsMesh->drawable = objLoader->getVBO("mars").lock();
    marsTexture->loadTexture(objLoader->getMaterial("mars_shader").map_Kd);
}

void setupShip(const std::weak_ptr<tigl::VBO> &vbo) {
    const auto &shipEntity = ecsCoordinator->createEntity();
    //Setup ship stuff
    const auto &transform = ecsCoordinator->addComponent<Transform>(shipEntity);
    const auto &mesh = ecsCoordinator->addComponent<Mesh>(shipEntity);
    const auto &textureComp = ecsCoordinator->addComponent<TextureComponent>(shipEntity);
    transform->scale = glm::vec3(0.1f);
    transform->rotation = fpscam->getRotation();
    mesh->drawable = vbo.lock();
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

    //Camera Light
    tigl::shader->setLightDirectional(0, false);
    tigl::shader->setLightAmbient(0, glm::vec3(0.25f, 0.25f, 0.25f));
    tigl::shader->setLightDiffuse(0, glm::vec3(0.9f, 0.9f, 0.9f));

    controlPanel->Init(glfwWindow);
    ecsCoordinator = std::make_unique<Coordinator>();
    ecsCoordinator->registerSystem<RenderSystem>();
    fpscam = std::make_unique<FPSCam>(glfwWindow);
    fpscam->setPosition(glm::vec3(10.f, 5.f, -10.f));
    objLoader = std::make_unique<ObjectLoader>();
    objLoader->loadObject("../resources/spaceship/lowpoly_spaceship.obj", "ship");
    objLoader->loadObject("../resources/spaceship/asteroid.obj", "asteroid");
    objLoader->loadObject("../resources/spaceship/sun.obj", "sun");
    objLoader->loadObject("../resources/spaceship/neptune.obj", "neptunus");
    objLoader->loadObject("../resources/spaceship/mars_planet.obj", "mars");
    setupShip(objLoader->getVBO("ship"));
    setupTerrain();
}


void printEntity(const std::weak_ptr<Entity> &entity) {
    if (entity.expired())
        return;
    std::cout << "Entity ID: " << entity.lock()->entityID << std::endl;

    std::cout << "Entity Signature: " << entity.lock()->getSig().to_string() << std::endl;
}

double lastTime = 0.0;
double applicationTime;
constexpr double asteroidFlyingRadius = 2.f;
constexpr double timeStep = 0.005;
bool frameIsStatic = false;

void asteroidRotation(const std::shared_ptr<Entity> &asteroid, const double &asteroidRadius, const double &constant) {
    const auto &asteroidTransform = asteroid->getComponent<Transform>();
    asteroidTransform->position = glm::vec3(glm::cos(applicationTime + constant) * asteroidRadius, 0,
                                            sin(applicationTime + constant) * asteroidRadius);
}

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
    const auto &sun = ecsCoordinator->getEntity(2);
    const auto &sunTransform = sun->getComponent<Transform>();
    const auto &light = sun->getComponent<LightComponent>();
    float distance = glm::distance(shipTransform->position, sunTransform->position);
    light->position = glm::vec3(distance / 2.f);

    const auto &neptune = ecsCoordinator->getEntity(6);
    const auto &neptuneTransform = neptune->getComponent<Transform>();
    neptuneTransform->rotation.y += deltaTime;

    //Asteroid ordnance.
    const auto &asteroid1 = ecsCoordinator->getEntity(3);
    asteroidRotation(asteroid1, asteroidFlyingRadius, 1.0);
    //Asteroid ordnance.
    const auto &asteroid2 = ecsCoordinator->getEntity(4);
    asteroidRotation(asteroid2, asteroidFlyingRadius, 3.0);
    //Asteroid ordnance.
    const auto &asteroid3 = ecsCoordinator->getEntity(5);
    asteroidRotation(asteroid3, asteroidFlyingRadius, 6.0);
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

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ecsCoordinator->getSystem<RenderSystem>()->draw();
    controlPanel->Render();
}