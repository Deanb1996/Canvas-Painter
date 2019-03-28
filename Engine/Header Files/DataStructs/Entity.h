#pragma once
#include "ComponentType.h"
#include <string>

struct Entity
{
	std::string mName;
	int mID;
	ComponentType mComponentMask;
};