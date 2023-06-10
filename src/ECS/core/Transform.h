#pragma once
#include "Component.h"
#include "glm/glm.hpp"

/* Transform Component
* Pure data for moving, rotating and scaling objects within the scene.
*/
struct Transform : public Component {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	/*
	* Base constructor which doesnt need parameters.
	*/
	Transform() {
		this->position = {};
		this->rotation = {};
		this->scale= {.5f, .5f, .5f};
	}

	/*
	* Overloaded version which can accept all parameters
	*/
	Transform(const glm::vec3& newPosition, const glm::vec3& newRotation, const glm::vec3& newScale) :
		position(newPosition), rotation(newRotation), scale(newScale) {}

	/*
	* Overloaded version which can accept a position and rotation parameters
	*/
	Transform(const glm::vec3& newPosition, const glm::vec3& newRotation) :
		position(newPosition), rotation(newRotation) {
		this->scale = { .5f, .5f, .5f };
	}

	/*
	* Overloaded version which can accept a position parameter
	*/
	Transform(const glm::vec3& newPosition) :
		position(newPosition) {
		this->rotation = { 0.f, 0.f, 0.f };
		this->scale = { .5f, .5f, .5f };
	}
};