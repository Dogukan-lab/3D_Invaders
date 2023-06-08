#pragma once
#include "System.h"
#include "../../components/Transform.h"

//TODO: Set bit signature men
class MoveSystem : public System {
	MoveSystem() {
		systemSignature.set(types::getComponentTypeID<Transform>());
	}
	void updateMovement();
};