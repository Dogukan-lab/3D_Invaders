#pragma once
#include <array>
#include <bitset>
#include <memory>

struct Component;

/*
* Used for types in the ECS.
*/
namespace types {
	constexpr int MAX_ENTITIES = 30;
	constexpr int MAX_COMPONENTS = 32;

	using EntityID = std::uint32_t;
	using ComponentID = std::uint8_t;
	using ComponentSignature = std::bitset<MAX_COMPONENTS>; //Signature is the combination of components an entity will represent.
	using ComponentArray = std::array<std::shared_ptr<Component>, MAX_COMPONENTS>;

	inline EntityID getUniqueEntityID() {
		static EntityID currentEntID = 0u;
		return currentEntID++;
	}

	inline ComponentID getUniqueComponentID() {
		static ComponentID currentCompID = 0u;
		return currentCompID++;
	}

	/*
	* Based on the component type the static ID will be retrieved, 
	* which in turn is used in the component array as a lookup variable.
	*/
	template<typename T>
	ComponentID getComponentTypeID() {
		static_assert(std::is_base_of_v<Component, T>, "Type is not of base Component!");
		static const ComponentID typeID = getUniqueComponentID();
		return typeID;
	}
}