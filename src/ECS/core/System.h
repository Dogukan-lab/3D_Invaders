#pragma once
#include "Types.h"
#include <vector>
class Entity;
//TODO Check if system can change to ID based instead of entity based.
/*
* Base class that stores entities aswell as assigning each system its own signature (Bitset).
*/
struct System {
	std::vector<std::shared_ptr<Entity>> entities;
	types::ComponentSignature systemSignature;
};