#include <memory>
#include <iostream>
class Entity;

/*
* Base class for all components
* Virtual deconstructor to have each component ID themselves before begin deleted.
* (Might remove the virtual deconstructor).
*/
struct Component {
public:
	Entity* entityRef;
	~Component() = default;

	void setEntityRef(Entity* ref) {
		this->entityRef = ref;
	}
};

