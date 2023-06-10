#pragma once
#include "Types.h"
#include <iostream>
/*
* Main class for creating entities (Might become base class  for other entities).
* Able to dynamically add components to the @compList through templates.
*/

class Entity {
public:
	types::EntityID entityID;

	Entity() {
		this->entityID = types::getUniqueEntityID();
		std::fill(this->compList.begin(), this->compList.end(), nullptr);
	}

	~Entity() {
		std::cout << "Entity destroyed! ID: " << this->entityID << std::endl;
	}

	/*
	* Adds component to array and sets bitflag for signature.
	* Checks if type is of base, if not throw an error! 
	* Change the static assert maybe? Because I believe this closes the program.
	*/
	template<typename T>
	std::shared_ptr<T> addComponent() {
		static_assert(std::is_base_of_v<Component, T>, "Type is not of Component base class!");
		auto ptr = std::make_shared<T>();
		
		ptr->setEntityRef(this);

		this->compList[types::getComponentTypeID<T>()] = ptr;
		this->signature[types::getComponentTypeID<T>()] = true;

		return ptr;
	}

	/*
	* Deletes component from array, updating the signature aswell.
	*/
	template<typename T> 
	void deleteComponent() {
		static_assert(std::is_base_of_v<Component, T>, "Type is not of Component base class!");
		this->compList[types::getComponentTypeID<T>()] = nullptr;
		this->signature[types::getComponentTypeID<T>()] = false;
	}

	//TODO create entity toString method!

	types::ComponentSignature& getSig() {
		return this->signature;
	}

	/*
	* Through casting it the desired can be gotten through a simple lookup!
	*/
	template<typename T>
	std::shared_ptr<T> getComponent() {
		static_assert(std::is_base_of_v<Component, T>, "Type is not base of component class!");
		return std::static_pointer_cast<T>(this->compList[types::getComponentTypeID<T>()]);
	}

private:
	types::ComponentArray compList;
	types::ComponentSignature signature;
};