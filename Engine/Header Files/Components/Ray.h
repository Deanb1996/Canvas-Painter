#pragma once
#include "MathsHelper.h"

struct Ray
{
	MathsHelper::Vector3 mOrigin;
	MathsHelper::Vector3 mDirection;
	MathsHelper::Vector3 mIntersectionPoint;
	int mIntersectedWith;
};