#pragma once

#include "gameObject.h"

class MovingObject : public GameObject
{
	public:
		MovingObject() = default;

		MovingObject(const std::string& dir, const std::string& fileName)
			: GameObject(dir, fileName)
		{
		}

		void Translate(float x, float y, float z);
		void SetPosition(const glm::vec3 pos);
		glm::vec3 GetPosition() const;
};
