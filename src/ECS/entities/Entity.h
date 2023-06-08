#pragma once
#include "../Types.h"
#include <unordered_map>

class Component;

class Entity {
public:
	//Maybe std::vector<std::pair<Comp, int>> compList?
	//std::unordered_map<std::shared_ptr<Component>, int> componentList;

	//Component bitset??? Voor systems om een sneller lookup te doen.
	Entity() {
		id = types::getUniqueEntityID();
	};

	template <typename T,typename ...TArgs>
	T& addComponent(TArgs&&... args) {
		static_assert(std::is_base_of<Component, T>::value, "Type is not base of Component");
		auto ptr = std::make_shared<T>(std::forward(args)...);

		this->compList[types::getComponentTypeID<T>()] = ptr;
		this->compSignature[types::getComponentTypeID<T>()] = true;

		return *ptr;
	}

	types::ComponentSignature& getBitset() {
		return this->compSignature;
	}

	//TODO Make this function for debug, do this later not now 06-06-2023.
	/*inline  getCompList() {
		return this->compList;
	}*/

	template <typename T>
	std::shared_ptr<T> getComponent() {
		static_assert(std::is_base_of<Component, T>::value, "Type is not base of Component");
		auto ptr = this->compList[types::getComponentTypeID<T>()];
		return std::static_pointer_cast<T>(ptr);
	}

	template <typename T>
	void deleteComponent() {
		this->compList.erase[types::getComponentTypeID<T>()];
	}

	//TODO Make entity printable for testing
	std::string entityToString() {
		std::string ent = this->compSignature.to_string();
		ent += this->id;
		return ent;
	}

private:
	std::unordered_map<types::ComponentID, std::shared_ptr<Component>> compList;
	types::ComponentSignature compSignature;
	types::EntityID id;
};