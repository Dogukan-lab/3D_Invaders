#pragma once

#include "Component.h"
#include "glm/glm.hpp"

class Transform : public Component {
public: 

	Transform() = default;

	Transform(const glm::vec3& newPos, const glm::vec3 newRotation)
		: position(newPos), rotation(newRotation)
	{}

	Transform(const glm::vec3& newPos,
		const glm::vec3& newRotation,
		const glm::vec3& newScale)
		: position(newPos), rotation(newRotation), scale(newScale)
	{}

	~Transform() override = default;

	inline glm::vec3& getPosition() {
		return this->position;
	}
	inline glm::vec3& getRotation() {
		return this->rotation;
	}
	inline glm::vec3& getScale() {
		return this->scale;
	}

private:
	glm::vec3 position = {};
	glm::vec3 rotation = {};
	glm::vec3 scale = {.5f, .5f, .5f};
};