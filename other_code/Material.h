#pragma once

#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.h"
#include <vector>

class Material
{
	public:
		Material(const std::string& matName);
		void SetTexture(const std::string& texPath);
		std::string GetTexturePath() const;
		void SetMaterialColor(const glm::vec4 color);

		void SelectMaterial();

		std::string GetName();

	private:
		std::string name;
		std::string texturePath;
		std::vector<Texture> textures;

		glm::vec4 diffuse{};
};
