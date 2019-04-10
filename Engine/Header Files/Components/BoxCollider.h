#pragma once
#include <vector>
#include "Vector3.h"

struct BoxCollider
{
	MathsHelper::Vector3 minBounds;
	MathsHelper::Vector3 maxBounds;
};