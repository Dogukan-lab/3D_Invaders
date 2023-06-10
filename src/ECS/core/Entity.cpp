#include "Entity.h"
#include <iostream>

Entity::Entity()
{
	this->entityID = types::getUniqueEntityID();
}

Entity::~Entity()
{
	std::cout << "Entity destroyed! ID: " << this->entityID << std::endl;
}
