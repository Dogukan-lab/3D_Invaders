#pragma once
#include <cstdint>
#include <memory>
#include <bitset>

namespace ECS{
	using EntityID = uint32_t;
	


	using ComponentID = uint32_t;
	constexpr ComponentID MAX_COMPONENTS = 32;
	static ComponentID getComponentID() {

		return 0;
	}
	using ComponentSignature = std::bitset<MAX_COMPONENTS>;

}