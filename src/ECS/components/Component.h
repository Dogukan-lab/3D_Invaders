#pragma once

struct Component {
public: 
	//std::weak_ptr<Entity> parentEntity;

	/*Component() {
		entity = nullptr;
	}*/

	Component() = default;

	virtual ~Component() = default;
};