#include "Overlay.h"

#include <iostream>

constexpr int NUMBER_WIDTH = 50;

void Overlay::AssignTexture(const std::string& numbersPath)
{
	textures.emplace_back(numbersPath);
}

void Overlay::Draw(const int score, const glm::vec3 cameraPos)
{
	//Shader settings
	tigl::shader->enableLighting(false);
	tigl::shader->enableAlphaTest(true);

	tigl::shader->enableColor(false);
	tigl::shader->enableTexture(true);
	tigl::shader->enableColorMult(false);

	//Start writing
	tigl::begin(GL_QUADS);
	textures[0].bind();

	//Constants
	constexpr float textureXOffset = NUMBER_WIDTH / (NUMBER_WIDTH * 10.0f);

	//Convert score to vector of digits
	const std::string scoreString = std::to_string(score);
	std::vector<int> digits = std::vector<int>();
	for (const char digit : scoreString)
		digits.push_back(digit - 48);

	//For each digit, draw digit
	for (size_t i = 0; i < digits.size(); i++)
	{
		const glm::vec3 positionOffset = glm::vec3(cameraPos.x - 5.5f + static_cast<float>(i), cameraPos.y + 2.f, -4);
		const float textureX = NUMBER_WIDTH * static_cast<float>(digits[i]) / (NUMBER_WIDTH * 10.0f);

		//Bottom Left
		addVertex(tigl::Vertex::PT(
				glm::vec3(0, 0, 0) + positionOffset,
				glm::vec2(textureX, 0))
		);

		//Bottom Right
		addVertex(tigl::Vertex::PT(
				glm::vec3(1, 0, 0) + positionOffset,
				glm::vec2(textureX + textureXOffset, 0))
		);

		//Top Right
		addVertex(tigl::Vertex::PT(
				glm::vec3(1, 1, 0) + positionOffset,
				glm::vec2(textureX + textureXOffset, 1))
		);

		//Top Left
		addVertex(tigl::Vertex::PT(
				glm::vec3(0, 1, 0) + positionOffset,
				glm::vec2(textureX, 1))
		);
	}

	//And and reset settings
	tigl::end();

	tigl::shader->enableLighting(true);
	tigl::shader->enableAlphaTest(false);
}
