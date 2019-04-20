#pragma once
#include "Vector3.h"

struct Ray
{
	KodeboldsMath::Vector3 origin;
	KodeboldsMath::Vector3 direction;
	KodeboldsMath::Vector3 intersectionPoint;
	int intersectedWith;
};