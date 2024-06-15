#pragma once
#include "Types.h"

/*
* Main class for creating entities (Might become base class  for other entities).
* Able to dynamically add components to the @compList through templates.
* TODO add a addNComponents template function to add multiple components at once.
*/
class Entity {
public:
	types::EntityID entityID{};

	Entity();
	~Entity();

	/*
	* Adds component to array and sets bitflag for signature.
	* Checks if type is of base, if not throw an error! 
	* Change the static assert maybe? Because I believe this closes the program.
	*/
	template<typename T>
	std::weak_ptr<T> addComponent() {
		static_assert(std::is_base_of_v<Component, T>, "Type is not of Component base class!");
		auto ptr = std::make_shared<T>();

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

	//TODO create entity toString method! If base class then make it overridable.

	inline types::ComponentSignature& getSig() {
		return this->signature;
	}

	/*
	* Through casting it to the desired component, it can be gotten through a simple lookup!
	*/
	template<typename T>
	std::weak_ptr<T> getComponent() {
		static_assert(std::is_base_of_v<Component, T>, "Type is not base of component class!");
		if (this->signature.test(types::getComponentTypeID<T>()))
			return std::static_pointer_cast<T>(this->compList[types::getComponentTypeID<T>()]);
		else
			return std::weak_ptr<T>();
	}

private:
	types::ComponentArray compList;
	types::ComponentSignature signature;
};