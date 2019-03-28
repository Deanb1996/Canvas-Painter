#pragma once
#include "Vector4.h"

struct Camera
{
	MathsHelper::Vector4 mLookAt;
	MathsHelper::Vector4 mUp;

	float mFOV;
	float mNear;
	float mFar;
};