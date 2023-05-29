#pragma once

#include <array>

struct Component;

class Entity {
public:

	//Component bitset??? Voor systems om een sneller lookup te doen.
	Entity() = default;

	template <typename T>
	T& addComponent(T& comp) {
		this->components.push_back(comp);

		return comp;
	}

	template <typename T>
	T& getComponent(T& component) {
		if (!component) return;

		return this->components[component];
	}

	template <typename T>
	void deleteComponent(T& comp) {
		if (!comp) return;
		this->components.erase[comp];
	}

private:
	std::array<Component*, 10> components;
};