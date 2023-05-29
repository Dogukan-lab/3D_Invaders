#pragma once

#include "include/Entity.h"
#include <vector>
#include <memory>
constexpr int MAX_ENTITIES = 10;

class EntityManager {
public:

	EntityManager() {
		this->entities.reserve(MAX_ENTITIES);
	}

	~EntityManager() {
		entities.clear();
	}

	/*template<typename T> 
	std::shared_ptr<Entity> addEntity() {
		static_assert(std::is_base_of<Entity, T>, "Entity is NULL!");

		this->entities.push_back(T);

		return std::make_shared<T>();
	}*/

	/*template <typename T, TArgs>
	void addEntity() {}*/

	void destroyEntity(const std::shared_ptr<Entity>& entity) {
		const auto ref = std::find(this->entities.begin(),
			this->entities.end(),
			entity); 

		if (ref != this->entities.end())
			this->entities.erase(ref);
	}

private:
	std::vector<std::shared_ptr<Entity>> entities;
};