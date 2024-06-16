#pragma once

#include "MovingObject.h"

class CompanionCube : public MovingObject
{
	public:
		CompanionCube() = default;

		CompanionCube(const std::string& dir, const std::string& fileName)
			: MovingObject(dir, fileName)
		{
		}

		bool IsGood() const;
		void SetGood(bool state);

	private:
		bool isGood = true;
};