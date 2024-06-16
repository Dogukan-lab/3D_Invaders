#include "CompanionCube.h"

bool CompanionCube::IsGood() const
{
	return isGood;
}

void CompanionCube::SetGood(const bool state)
{
	isGood = state;
}