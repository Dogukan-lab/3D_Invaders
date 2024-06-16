#pragma once
#include "Component.h"
#include "glad/gl.h"
#include <string>

struct TextureComponent : public Component {
	GLuint textureID{};
    std::string texPath;

	TextureComponent() = default;

	void loadTexture(const std::string& texturePath);
	void bindTexture() const;
    void unbindTexture() const;
};