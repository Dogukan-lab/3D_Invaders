#include "CompanionCube.h"

#include <string>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <exception>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Overlay.h"

#define INCLUDE_TESTS 0
#if INCLUDE_TESTS == 1
#include <gtest/gtest.h>
#include <chrono>
#pragma warning(disable : 4996)
#endif

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

//Methods
void Init(int argc, char** argv);

void Update();
void UpdateCamera(clock_t delta);
void UpdateCubes(const clock_t delta);
void UpdateCrusher(const clock_t delta);
void Draw();

void CheckForCrush();
void EndGame();
void GetObjectDir();

//GameVars
clock_t lastDeltaTime = 0;
clock_t fpsCounter = 0;
int fps = 0;
int score = 0;

Overlay overlay;

//Cubes
int timeSinceLastCube = 0;
int badCubeChance = 40; //Out of 100
float cubeSpeed = 1;
int cubeCooldown = 2000; //In milliseconds
constexpr int BAD_CUBE_CHANCE_CAP = 75;
constexpr int CUBE_SPEED_CAP = 6;
constexpr int CUBE_COOLDOWN_CAP = 450;
constexpr int CUBE_SIZE_OFFSET = 2;

//Crusher
bool lastClick = false;
float crushRange = 1;
constexpr float CRUSH_RANGE_CAP = 2;

constexpr float CRUSHER_DEFAULT_HEIGHT = 1;
constexpr float CRUSHER_CRUSH_HEIGHT = -3.25;
float crusherTargetHeight = CRUSHER_DEFAULT_HEIGHT;
float crusherHeight = crusherTargetHeight;

constexpr float CRUSHER_DEFAULT_SPEED = 1;
constexpr float CRUSHER_CRUSH_SPEED = 10;
float crusherSpeed = CRUSHER_DEFAULT_SPEED;

//Window
#define ENABLE_FULLSCREEN 0;
constexpr float WINDOWED_SIZE = 0.75f;
int windowWidth = static_cast<int>(1920 * WINDOWED_SIZE);
int windowHeight = static_cast<int>(1080 * WINDOWED_SIZE);
GLFWwindow* window;

//Camera
glm::vec3 cameraTarget = glm::vec3(0, 0, 0);
glm::vec3 cameraPos = glm::vec3(0, 2, 0);
constexpr glm::vec3 CAMERA_OFFSET = glm::vec3(0, 0, 10);
constexpr float CAMERA_SENSITIVITY = 1.75f;

//Objects
std::string workingDirectory;
constexpr double LEVEL_WIDTH = 40;

std::vector<CompanionCube> companionCubes = std::vector<CompanionCube>();
std::vector<GameObject> baseObjects = std::vector<GameObject>();
CompanionCube goodCube = CompanionCube();
CompanionCube badCube = CompanionCube();
MovingObject crusher = MovingObject();

int main(const int argc, char** argv)
{
	GetObjectDir();

	//Test initiate glfw
	if (!glfwInit())
		throw std::runtime_error("Could not initialize glwf");

	//Get primary monitor size so that the fullscreen application can have the correct resolution
	GLFWmonitor* monitor = nullptr;
#if ENABLE_FULLSCREEN == 1
		monitor = glfwGetPrimaryMonitor();
		glfwGetMonitorWorkarea(monitor, nullptr, nullptr, &windowWidth, &windowHeight);
#endif

	window = glfwCreateWindow(windowWidth, windowHeight, "CPP 3D Project", monitor, nullptr);

	//Check if the window was successfully made
	if (!window)
	{
		glfwTerminate();
		throw std::runtime_error("Could not initialize glwf");
	}
	glfwMakeContextCurrent(window);

	//Escape key callback to quit application
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(window, true);
	});

	//Init
	Init(argc, argv);

	//MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		//Program cycle
		Update();
		Draw();

		//glfw cycle
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Termination
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void Init(int argc, char** argv)
{
#if INCLUDE_TESTS == 1
	//Testing
	testing::InitGoogleTest(&argc, argv);

	std::ofstream writer;
	writer.open("testLog.txt", std::ios::app);
	if (writer.is_open())
	{
		const long long time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		std::string result = RUN_ALL_TESTS() ? "Failure! Test ran unsuccessfully." : "Success! Object was correctly translated.";
		writer << "\n" << result.c_str() << ctime(&time);
		writer.close();
	}
#endif

	//Init
	tigl::init();
	glfwSetCursorPos(window, static_cast<double>(windowWidth) / 2, static_cast<double>(windowHeight) / 2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	overlay.AssignTexture(workingDirectory + "objects/UI/numbers.png");

	//GL settings
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 1);

	//Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Viewport
	tigl::shader->setProjectionMatrix(glm::perspective(
		glm::radians(75.f),
		static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
		0.1f,
		100.f
	));

	//Shader settings
	tigl::shader->enableLighting(true);
	tigl::shader->setLightCount(1);
	tigl::shader->setShinyness(25);

	//Camera Light
	tigl::shader->setLightDirectional(0, false);
	tigl::shader->setLightAmbient(0, glm::vec3(0.25f, 0.25f, 0.25f));
	tigl::shader->setLightDiffuse(0, glm::vec3(0.9f, 0.9f, 0.9f));

	//Base scene objects
	GameObject belt = GameObject(-CAMERA_OFFSET);
	belt.LoadObjectFile(workingDirectory, "Belt");
	baseObjects.push_back(belt);
	GameObject start = GameObject(-CAMERA_OFFSET);
	start.LoadObjectFile(workingDirectory, "Start");
	baseObjects.push_back(start);
	GameObject end = GameObject(-CAMERA_OFFSET);
	end.LoadObjectFile(workingDirectory, "End");
	baseObjects.push_back(end);

	//Reference cubes
	goodCube.LoadObjectFile(workingDirectory, "GoodCube");
	badCube.LoadObjectFile(workingDirectory, "BadCube");

	//Crusehr
	crusher.LoadObjectFile(workingDirectory, "Crusher");
}

void Update()
{
	//Clock speed
	const clock_t currentTime = clock();
	clock_t deltaTime = currentTime - lastDeltaTime;
	lastDeltaTime = currentTime;

	//FPS
	fpsCounter += deltaTime;
	fps++;
	if (fpsCounter >= 1000)
	{
		std::cout << "FPS: " << fps << std::endl;
		fps = 0;
		fpsCounter = 0;
	}

	//Update cycle
	UpdateCamera(deltaTime);
	UpdateCubes(deltaTime);
	UpdateCrusher(deltaTime);
}

void UpdateCamera(const clock_t delta)
{
	//Get cursor position
	double cursorX;
	glfwGetCursorPos(window, &cursorX, nullptr);

	//Convert window position to game-word position (clamped)
	double currentPos = cursorX / static_cast<double>(windowWidth);
	if (currentPos < 0) currentPos = 0;
	else if (currentPos > 1) currentPos = 1;

	//Set target
	cameraTarget = glm::vec3(LEVEL_WIDTH * currentPos - LEVEL_WIDTH / 2, 0, 0);

	//Calculate camera position
	const float difference = cameraTarget.x - cameraPos.x;
	if (abs(difference) < 0.01f)
	{
		cameraPos = cameraTarget;
		cameraPos.y = 2;
	}
	else cameraPos += glm::vec3(difference * CAMERA_SENSITIVITY / static_cast<float>(delta), 0, 0);

	//Clamp position again for the really rare glitch if the game lags hard on startup
	constexpr double halfSize = LEVEL_WIDTH / 2;
	if (cameraPos.x < static_cast<float>(-halfSize)) cameraPos.x = static_cast<float>(-halfSize);
	else if (cameraPos.x > static_cast<float>(halfSize)) cameraPos.x = halfSize;


	//Camera position
	tigl::shader->setViewMatrix(glm::lookAt(
		cameraPos,
		cameraPos - CAMERA_OFFSET,
		glm::vec3(0, 1, 0)
	));

	//Light position
	tigl::shader->setLightPosition(0, cameraPos);
}

void UpdateCubes(const clock_t delta)
{
	//Check if cube needs to be spawned
	timeSinceLastCube += delta;
	if (timeSinceLastCube >= cubeCooldown)
	{
		//Reset vars
		srand(clock());
		timeSinceLastCube = 0;

		//Randomize if cube will be good or bad
		const bool isGood = rand() % 100 > badCubeChance;

		//Spawn cube
		CompanionCube cube;
		if (isGood) cube = CompanionCube(goodCube);
		else
		{
			cube = CompanionCube(badCube);
			cube.SetGood(false);
		}

		cube.Translate(-LEVEL_WIDTH / 2 - CUBE_SIZE_OFFSET, 0, -CAMERA_OFFSET.z);
		companionCubes.push_back(cube);
	}

	//Move cubes an remove if cube has gone offscreen
	std::vector<int> indicesToRemove = std::vector<int>();
	for (int i = 0; i < companionCubes.size(); i++)
	{
		//Move
		companionCubes[i].Translate(cubeSpeed / static_cast<float>(delta), 0, 0);

		//Check if offscreen
		if (companionCubes[i].GetPosition().x > static_cast<float>(LEVEL_WIDTH / 2) + CUBE_SIZE_OFFSET)
		{
			//Check if a bad cube reached the end
			if (!companionCubes[i].IsGood()) EndGame();
			else indicesToRemove.push_back(i);
		}
	}

	//Remove all offscreen cubes
	for (const int i : indicesToRemove)
		companionCubes.erase(companionCubes.begin() + i);
}

void UpdateCrusher(const clock_t delta)
{
	//Mouse click action
	const bool hasClicked = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (hasClicked != lastClick)
	{
		lastClick = hasClicked;

		//If clicked, set all targets
		if (hasClicked)
		{
			crusherTargetHeight = CRUSHER_CRUSH_HEIGHT;
			crusherSpeed = CRUSHER_CRUSH_SPEED;
		}
	}

	//Calculate difference
	const float difference = crusherTargetHeight - crusherHeight;

	//If target has been reached: reset targets do default and check for crush. Else make a move
	if (abs(difference) < 0.01f)
	{
		if (crusherTargetHeight == CRUSHER_CRUSH_HEIGHT) CheckForCrush();

		crusherHeight = crusherTargetHeight;
		crusherTargetHeight = CRUSHER_DEFAULT_HEIGHT;
		crusherSpeed = CRUSHER_DEFAULT_SPEED;
	}
	else crusherHeight += difference * crusherSpeed / static_cast<float>(delta);

	//Crusher movement
	crusher.SetPosition(
		cameraTarget
		+ glm::vec3(0, crusherHeight, 0)
		- CAMERA_OFFSET
	);
}

void Draw()
{
	//Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Moving Objects
	for (const auto& companionCube : companionCubes)
	{
		companionCube.Draw();
	}

	//Base scene objects
	for (const auto& baseObject : baseObjects)
	{
		baseObject.Draw();
	}

	crusher.Draw();

	overlay.Draw(score, cameraPos);
}

void CheckForCrush()
{
	const float crushX = cameraTarget.x;
	std::vector<int> cubesToCrush = std::vector<int>();

	//Check crush for all cubes
	for (int i = 0; i < companionCubes.size(); i++)
	{
		//On crush
		if (abs(companionCubes[i].GetPosition().x - crushX) < crushRange)
		{
			//Check if crushed cube was good or bad
			if (companionCubes[i].IsGood()) EndGame();
			else
			{
				//Increase gameVars
				score++;

				badCubeChance += 2;
				cubeSpeed += 0.2f;
				cubeCooldown -= 75;
				crushRange += 0.05f;

				//Instant max for testing
				// badCubeChance = BAD_CUBE_CHANCE_CAP;
				// cubeSpeed = CUBE_SPEED_CAP;
				// cubeCooldown = CUBE_COOLDOWN_CAP;
				// crushRange = CRUSH_RANGE_CAP;

				//Cap gameVars
				if (badCubeChance > BAD_CUBE_CHANCE_CAP) badCubeChance = BAD_CUBE_CHANCE_CAP;
				if (cubeSpeed > CUBE_SPEED_CAP) cubeSpeed = CUBE_SPEED_CAP;
				if (cubeCooldown < CUBE_COOLDOWN_CAP) cubeCooldown = CUBE_COOLDOWN_CAP;
				if (crushRange > CRUSH_RANGE_CAP) crushRange = CRUSH_RANGE_CAP;

				//Crush cube
				cubesToCrush.push_back(i);
			}
		}
	}

	for (const int& i : cubesToCrush)
		companionCubes.erase(companionCubes.begin() + i);
}

void EndGame()
{
	std::string recordString;

	//Open reading stream
	std::ifstream reader;
	reader.open("Record.txt", std::ios::in);
	if (reader.is_open())
	{
		//Read previous record
		std::string line;
		std::getline(reader, line);
		const int record = atoi(line.c_str());
		reader.close();

		recordString += "Previous record: " + std::to_string(record);

		//If new record
		if (score > record)
		{
			//Write record to file
			std::ofstream writer;
			writer.open("Record.txt", std::ios::out);
			if (writer.is_open()) {
				writer << score;
				writer.close();
			} else
			{
				throw std::runtime_error("Failed to write to Record File");
			}

			recordString += "\nNEW RECORD!";
		}
	}
	else {
		reader.close();
		throw std::runtime_error("Failed to read Record File");
	}

	//Print round information
	std::cout << "----- GAME OVER -----" << "\n";
	std::cout << "Score: " << score << "\n";
	std::cout << recordString << std::endl;
	glfwSetWindowShouldClose(window, true);
}

void GetObjectDir()
{
	const std::string target = "CPP_3D_Project";
	const std::string exe = ".exe";
	std::vector<std::string> dirs = std::vector<std::string>();
	dirs.emplace_back("x64\\Release");
	dirs.emplace_back("x64\\Debug");
	dirs.emplace_back("32\\Release");
	dirs.emplace_back("x64\\Debug");

	bool isInProjDir = false;

	char buffer[MAX_PATH];
	GetModuleFileNameA(nullptr, buffer, MAX_PATH);

	//Convert buffer to string
	std::string path;

	//Convert buffer to string (keep going till "exe" is found)
	for (const char letter : buffer)
	{
		//Add
		path += letter;

		for (const auto& dir : dirs)
		{
			if (path.length() > dir.size())
			{
				std::string end = path.substr(path.length() - dir.size(), path.length());
				if (end == dir)
				{
					isInProjDir = true;
					path = path.substr(0, path.length() - end.length());
					break;
				}
			}
		}
		if (isInProjDir) break;

		//Find "exe"
		if (path.length() >= exe.size())
		{
			std::string end = path.substr(path.length() - exe.size(), path.length() - 0);
			if (end == exe)
			{
				isInProjDir = false;
				path = path.substr(0, path.length() - target.length() - exe.length());
				break;
			}
		}
	}

	workingDirectory = path;
	std::cout << "Path: " << workingDirectory << std::endl;
	std::cout << "Dir: " << (isInProjDir ? "Yes" : "No") << std::endl;
}
