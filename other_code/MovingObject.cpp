#include "MovingObject.h"

void MovingObject::Translate(const float x, const float y, const float z)
{
	transform += glm::vec3(x, y, z);
}

void MovingObject::SetPosition(const glm::vec3 pos)
{
	transform = pos;
}

glm::vec3 MovingObject::GetPosition() const
{
	return transform;
}