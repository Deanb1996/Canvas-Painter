#pragma once
#include <vector>
#include "Vector3.h"

struct BoxCollider
{
	MathsHelper::Vector3 mMinBounds;
	MathsHelper::Vector3 mMaxBounds;
};