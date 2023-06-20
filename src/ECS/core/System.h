#pragma once
#include "Types.h"
#include <vector>
class Entity;
//TODO Check if system can change to ID based instead of entity based.
/*
* Base class that stores entities aswell as assigning each system its own signature (Bitset).
*/
struct System {
	//TODO consider making this into an array, map, or even an unorderedmap.
	//You already have the entityID ready and everything.
	std::vector<std::shared_ptr<Entity>> entities {};
	types::ComponentSignature systemSignature;
};