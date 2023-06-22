#pragma once
#include <memory>
#include <iostream>
class Entity;

/*
* Base class for all components
* Virtual de-constructor to have each component ID themselves before being deleted.
* (Might remove the virtual de-constructor).
*/
struct Component {
	~Component() = default;
};

