#include "SystemManager.h"
#include "Entity.h"
#include <iostream>

void SystemManager::entitySignatureChanged(std::shared_ptr<Entity> entity)
{
	for (const auto& systemPair : this->systems) {
		auto system = systemPair.second;
		if ((entity->getSig() & system->systemSignature) == system->systemSignature) {
			std::cout << "Entity adding into system!" << system->systemSignature.to_string() << "\n";
			system->entities.push_back(entity);
		}
		else {
			std::cout << "Entity not in system!" << entity->entityID << "Sys sig: " << system->systemSignature.to_string() << "\n";
			system->entities.erase(
				std::remove(system->entities.begin(), system->entities.end(), entity),
				system->entities.end());
		}
	}
}

void SystemManager::destroyEntity(std::shared_ptr<Entity> entity)
{
	for (const auto& systemPair : this->systems) {
		const auto& system = systemPair.second;

		system->entities.erase(
			std::find(
				system->entities.begin(),
				system->entities.end(),
				entity
			)
		);
	}
}
