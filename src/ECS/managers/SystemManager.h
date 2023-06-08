#pragma once 

#include "../system/include/System.h"
class Entity;
/*
* 
* This manager updates the systems as new entities get added to the system.
* Sorta like a callback structure wherein the manager only sets the system signature 
when the entity has any of them.
* 
*/
class SystemManager {
public:

	SystemManager() = default; 
	
	/*
	* Registers the new system in the manager,
	*/
	template <typename T>
	T& registerSystem() {
		static_assert(std::is_base_of<System, T>::value, 
			"Provided System is not a derivative of system!");
		auto systemPtr = std::make_shared<T>();
		static_assert(std::find(
			this->systems.begin(),
			this->systems.end(),
			systemPtr
		), "System already registered!");

		this->systems.push_back(systemPtr);
		return *systemPtr;
	}

	/*
	* Removes the removed entity from any systems associated with the entity.
	*/
	void entityDestroyed(const std::shared_ptr<Entity> entity);

	/*
	* If an Entity gets added, or its signature changes this function is used to add
	* its components to the required systems.
	*/
	void entitySignatureChanged(const std::shared_ptr<Entity> entity);
	

private:
	std::vector<std::shared_ptr<System>> systems;
};