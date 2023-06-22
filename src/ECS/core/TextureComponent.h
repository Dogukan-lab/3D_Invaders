#pragma once
#include "Component.h"


struct TextureComponent : public Component {
	unsigned int textureID;

	TextureComponent() = default;

	void loadTexture(const char* texturePath);
	void bindTexture() const;
};