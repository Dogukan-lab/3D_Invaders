#pragma once
#include <vector>

#include "Texture.h"
#include "tigl.h"

class Overlay
{
	public:
		Overlay() = default;
		void AssignTexture(const std::string& numbersPath);
		void Draw(int score, glm::vec3 cameraPos);

	private:
		std::vector<Texture> textures;
};
