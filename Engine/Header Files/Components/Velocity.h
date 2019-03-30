#pragma once
#include <DirectXMath.h>
#include "Vector4.h"

struct Velocity
{
	MathsHelper::Vector4 mVelocity;
	MathsHelper::Vector4 mAcceleration;
	float maxSpeed;
};
