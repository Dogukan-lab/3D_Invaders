#pragma once

#include "../entities/Entity.h"

class System {
	std::vector<std::shared_ptr<Entity>> entities;
	types::ComponentSignature systemSignature;
};