#pragma once
#include <DirectXMath.h>
#include "Vector4.h"

struct Velocity
{
	MathsHelper::Vector4 velocity;
	MathsHelper::Vector4 acceleration;
	float maxSpeed;
};
