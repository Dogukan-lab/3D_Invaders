#pragma once

#include "EntityManager.h"
#include "SystemManager.h"

struct Transform;
struct Mesh;

/*
* API esque interface
* Easily add entity type to all necessary managers.
* Remove entity type from all necessary managers.
* May modify components if implemented.
*/
class Coordinator {
public:
	Coordinator() {
		this->entityManager = new EntityManager();
		this->systemManager = new SystemManager();
	}

	//TODO if entity types introduced then make this templated!
	/*
	* Adds entity to all managers, for now uses a standard entity component preset.
	*/
	std::shared_ptr<Entity> createEntity() {
		auto entity = this->entityManager->createEntity();
		return entity;
	}

	std::shared_ptr<Entity> getEntity(const types::EntityID& entityID) {
		return this->entityManager->getEntity(entityID);
	}

	void destroyEntity(std::shared_ptr<Entity> entity) {
		this->entityManager->destroyEntity(entity->entityID);
		this->systemManager->destroyEntity(entity);
	}

	template<typename T>
	std::shared_ptr<T> addComponent(const int& entityID) {
		auto entity = this->entityManager->getEntity(entityID);
		entity->addComponent<T>();
		this->systemManager->entitySignatureChanged(entity);
		return entity->getComponent<T>();
	}

	template<typename T>
	void removeComponent(const types::EntityID& entityID) {
		auto entity = this->entityManager->getEntity(entityID);
		entity->deleteComponent<T>();
		this->systemManager->entitySignatureChanged(entity);
	}

	template<typename T>
	std::shared_ptr<T> registerSystem() {
		return this->systemManager->registerSystem<T>();
	}
	
	template<typename T>
	std::shared_ptr<T> getSystem() {
		return this->systemManager->getSystem<T>();
	}

	//TODO find a way that the scene can be drawn with forward declarations
	//void drawScene() {
	//	this->systemManager->getSystem<RenderSystem>()->draw();
	//}

private:
	EntityManager* entityManager;
	SystemManager* systemManager;
};