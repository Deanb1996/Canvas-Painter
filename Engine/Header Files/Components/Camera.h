#pragma once
#include "Vector4.h"

struct Camera
{
	KodeboldsMath::Vector4 lookAt;
	KodeboldsMath::Vector4 up;

	float FOV;
	float nearPlane;
	float farPlane;
};