#pragma once

#include "managers/EntityManager.h"
#include "managers/SystemManager.h"

/*
* This class updates entities that are newly added to the environment.
*/
class Coordinator {
public:

	Coordinator() {
		this->entityManager = std::make_unique<EntityManager>();
		this->systemManager = std::make_unique<SystemManager>();
	}

	//Set signature for system
	Entity& createEntity() {
		const auto& entity = this->entityManager->addEntity();
		this->systemManager->entitySignatureChanged(entity);
		return ;
	}

	void destroyEntity(const std::shared_ptr<Entity>& entity) {
		this->entityManager->destroyEntity(entity);
		this->systemManager->entityDestroyed(entity);
	}

	

	template<typename T>
	std::shared_ptr<T> registerSystem() {
		static_assert(std::is_base_of_v<System, T>, "Type is not of base System!");

		auto ptr = std::make_shared<T>();

		this->systemManager->registerSystem(ptr);

		return 
	}

private:
	std::unique_ptr<EntityManager> entityManager;
	std::unique_ptr<SystemManager> systemManager;
};