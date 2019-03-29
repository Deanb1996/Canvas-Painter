#pragma once
#include <DirectXMath.h>
#include "Vector4.h"

struct Velocity
{
	KodeBoldsMath::Vector4 mVelocity;
	KodeBoldsMath::Vector4 mAcceleration;
	float maxSpeed;
};
