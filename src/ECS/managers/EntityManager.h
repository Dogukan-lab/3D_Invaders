#pragma once

#include "../entities/Entity.h"
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

	//Function simply adds an entity to the manager.
	template <typename T>
	Entity& addEntity() {
		static_assert(std::is_base_of<Entity, T>::value, "Type is not base of Entity!");
		auto ptr = std::make_shared<T>();

		this->entities.push_back(ptr);
		return *ptr;
	}

	Entity& addEntity() {
		//static_assert(std::is_base_of<Entity, T>::value, "Type is not base of Entity!");
		auto ptr = std::make_shared<Entity>();

		this->entities.push_back(ptr);
		return *ptr;
	}

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