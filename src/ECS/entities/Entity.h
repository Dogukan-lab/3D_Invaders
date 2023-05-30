#pragma once
#include "../Types.h"
#include <unordered_map>

struct Component;

class Entity {
public:
	//Maybe std::vector<std::pair<Comp, int>> compList?
	//std::unordered_map<std::shared_ptr<Component>, int> componentList;

	//Component bitset??? Voor systems om een sneller lookup te doen.
	Entity() {
		this->componentList.reserve(types::MAX_COMPONENTS);
	}

	std::bitset& getBitset() {
		return this->compSignature;
	}

	template <typename T>
	std::shared_ptr<T>& addComponent() {
		static_assert(std::is_base_of<Component, T>::value, "Type is not base of Component");
		auto ptr = std::make_shared<T>();
		this->compList.push_back(ptr);
		this->compSignature[types::getComponentTypeID<T>()] = true;

		return ptr;
	}

	template <typename T>
	std::shared_ptr<T>& getComponent(T& component) {
		static_assert(std::is_base_of<Component, T>::value, "Type is not base of Component");

		return this->components[component];
	}

	template <typename T>
	void deleteComponent(T& comp) {
		if (!comp) return;
		this->components.erase[comp];
	}

private:
	std::vector<std::shared_ptr<Component>> compList;
	types::ComponentSignature compSignature;
};