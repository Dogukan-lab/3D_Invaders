#pragma once
#include "Entity.h"
#include <map>
/*
* The manager is used to keep tabs on entities that have been spawned in the world.
* If an entity "dies" or goes out of scope it goes through the manager first to destroy the entity.
*/
class EntityManager {
public:

	EntityManager() {
		this->entities = {};
	}

	/*
	* Function for adding entities.
	* TODO make overloaded function that functions with a template instead, 
	* only needed when entities become dynamic for e.g. player, worldObject and so on.
	*/
	std::shared_ptr<Entity> createEntity() {
		auto ptr = std::make_shared<Entity>();
		this->entities[ptr->entityID] = ptr;

		return ptr;
	}

	/*
	* Destroys entity based on the id.
	*/
	void destroyEntity(const types::EntityID& entityID) {
		this->entities.erase(entityID);
	}

	std::shared_ptr<Entity> getEntity(const types::EntityID& entityID) {
		return this->entities[entityID];
	}

private:
	//Map for unique entityIDs as well as storing the entities.
	std::map<types::EntityID, std::shared_ptr<Entity>> entities;
};