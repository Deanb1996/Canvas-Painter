#pragma once
#include "Vector4.h"

struct Camera
{
	MathsHelper::Vector4 lookAt;
	MathsHelper::Vector4 up;

	float FOV;
	float nearPlane;
	float farPlane;
};