#pragma once
#include "../../Types.h"
#include <set>

class Entity;
class System {
public:
	//TODO maybe change it into a unordered map for easy lookup with the entityID.
	std::set<std::shared_ptr<Entity>> entities;
	types::ComponentSignature systemSignature;
};