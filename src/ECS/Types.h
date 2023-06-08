#pragma once
#include <memory>
#include <bitset>


//TODO: Check if EntityID is necessary.
namespace types{
	using EntityID = std::uint32_t;
	constexpr EntityID MAX_ENTITIES = 10;

	using ComponentID = std::uint32_t;
	constexpr ComponentID MAX_COMPONENTS = 32;
	using ComponentSignature = std::bitset<MAX_COMPONENTS>;
	//using ComponentArray = std::array<std::shared_ptr<Component>, MAX_COMPONENTS>;

	static EntityID getUniqueEntityID() {
		static EntityID currentEID = 0u;
		return currentEID++;
	}

	inline ComponentID getUniqueComponentID() {
		static ComponentID currentID = 0u;
		return currentID++;
	}
	
	template <typename T> ComponentID getComponentTypeID() {
		static_assert(std::is_base_of_v<Component, T>, "Provided Component is not a derivative of Component!");
		static const ComponentID typeID = getUniqueComponentID();
		return typeID;
	}
}