#pragma once
#include "System.h"
#include <map>
/*
* SystemManager keeps tabs on every system running.
* Adds sytems by registering them.
* Adds entities to systems when bitsets "overlap".
* Also needs to be able to update the list of entities when an entity is destroyed.
* TODO Set system signature through coordinator perhaps?
*/
class SystemManager {
public:
	SystemManager() = default;

	/**
	* @brief Registers a system to the manager.
	* Able to add enties to system through the manager now.
	* When creating the system its name gets passed as the key and the system is the value.
	* TODO Consider making this a void function for you don't use it at all for the return value.
	*/
	template<typename T>
	std::shared_ptr<T> registerSystem() {
		static_assert(std::is_base_of_v<System, T>, "Type is not of base System!");
		const char* typeName = typeid(T).name();
		
		auto system = std::make_shared<T>();

		this->systems.insert({typeName, system});
		return system;
	}

	/*
	* Checks for every system if the entity needs to be added to said system.
	* Checking if that is true through bitset comparison. 
	* If not then remove this entity from the system (if it was inserted before no longer having the necessary bitset).
	* TODO when you have time improve this lookup ples.
	*/
	void entitySignatureChanged(std::shared_ptr<Entity> entity);

	void destroyEntity(std::shared_ptr<Entity> entity);

	template<typename T>
	std::shared_ptr<T> getSystem() {
		static_assert(std::is_base_of_v<System, T>, "Type is not of base system!");
		return std::static_pointer_cast<T>(this->systems[typeid(T).name()]);
	}

private:
	//Name, Type
	std::map<const char*, std::shared_ptr<System>> systems;
};