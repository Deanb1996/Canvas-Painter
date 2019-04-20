#pragma once
#include "Vector4.h"

struct Velocity
{
	KodeboldsMath::Vector4 velocity;
	KodeboldsMath::Vector4 acceleration;
	float maxSpeed;
};
