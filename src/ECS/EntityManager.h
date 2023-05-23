#pragma once

#include "include/Entity.h"
#include <vector>
//#include <queue>
#include <memory>

constexpr int MAX_ENTITIES = 10;

class EntityManager {
public:

	EntityManager() {
		this->entities.reserve(MAX_ENTITIES);
	}

	EntityManager(const int& amount) {
		entities.reserve(amount);
	}

	~EntityManager() {
		entities.clear();
	}

	void destroyEntity(const std::shared_ptr<Entity>& entity) {
		//static_assert(std::is_base_of)
		const auto ref = std::find(this->entities.begin(),
			this->entities.end(),
			entity); 

		if (ref != this->entities.end())
			this->entities.erase(ref);
	}





private:
	std::vector<std::shared_ptr<Entity>> entities;
};