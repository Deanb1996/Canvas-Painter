#pragma once
#include <vector>
#include "Vector3.h"

struct BoxCollider
{
	KodeboldsMath::Vector3 minBounds;
	KodeboldsMath::Vector3 maxBounds;
};