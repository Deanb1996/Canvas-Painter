#pragma once
#include "MathsHelper.h"

struct Ray
{
	MathsHelper::Vector3 origin;
	MathsHelper::Vector3 direction;
	MathsHelper::Vector3 intersectionPoint;
	int intersectedWith;
};