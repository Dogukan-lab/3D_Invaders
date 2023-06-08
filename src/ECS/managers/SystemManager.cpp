#include "SystemManager.h"
#include "../entities/Entity.h"

void SystemManager::entityDestroyed(const std::shared_ptr<Entity> entity)
{
	for (const auto& system : this->systems) {
		system->entities.erase(entity);
	}
}

void SystemManager::entitySignatureChanged(const std::shared_ptr<Entity> entity)
{
	for (const auto& system : this->systems) {
		const auto& sysSignature = system->systemSignature;
		const auto& entitySig = entity->getBitset();
		if ((entitySig & sysSignature) == sysSignature) {
			//Entity heeft benodigde componenten dus voeg toe aan system.
			system->entities.insert(entity);
		}
		else {
			system->entities.erase(entity);
		}
	}
} 
