#include "Material.h"
#include "tigl.h"

#include <iostream>

Material::Material(const std::string& matName)
{
	name = matName;
}

void Material::SetTexture(const std::string& texPath)
{
	texturePath = texPath;

	textures.clear();
	textures.emplace_back(texturePath);
}

std::string Material::GetTexturePath() const
{
	return texturePath;
}

void Material::SetMaterialColor(const glm::vec4 color)
{
	diffuse = color;
}

void Material::SelectMaterial()
{
	if (!textures.empty())
	{
		tigl::shader->enableColor(false);
		tigl::shader->enableTexture(true);
		tigl::shader->enableColorMult(false);

		textures[0].bind();
	}
	else
	{
		tigl::shader->enableColor(false);
		tigl::shader->enableTexture(false);
		tigl::shader->enableColorMult(true);
		
		tigl::shader->setColorMult(diffuse);
	}
}

std::string Material::GetName()
{
	return name;
}
