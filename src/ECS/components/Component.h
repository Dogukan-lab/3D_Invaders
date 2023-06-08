#pragma once

#include "../entities/Entity.h"

class Component {
public:
	std::weak_ptr<Entity> coupledEnt;
	virtual ~Component() {};
};