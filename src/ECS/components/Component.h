#pragma once

class Entity;

struct Component {
	std::weak_ptr<Entity> coupledEnt;

	Component() = default;

	virtual ~Component() = default;
};