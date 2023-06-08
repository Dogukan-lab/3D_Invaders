#include "include/MoveSystem.h"
#include <iostream>

void MoveSystem::updateMovement()
{
	for (const auto& entity : this->entities) {
		auto comp = entity->getComponent<Transform>();
		std::cout << "Transform position: X:" << comp->getPosition().x 
			<< " Y:" << comp->getPosition().y 
			<< " Z:" << comp->getPosition().z << "\n";
	}
}
