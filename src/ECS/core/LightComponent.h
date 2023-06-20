#pragma once
#include "Component.h"
#include "glm/glm.hpp"

struct LightComponent : public Component {
	glm::vec3 position;
};