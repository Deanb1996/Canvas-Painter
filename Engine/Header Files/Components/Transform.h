#pragma once
#include "Vector4.h"
#include "Matrix4.h"

struct Transform
{
	KodeboldsMath::Matrix4 transform;
	KodeboldsMath::Vector4 translation;
	KodeboldsMath::Vector4 rotation;
	KodeboldsMath::Vector4 scale;
	KodeboldsMath::Vector4 forward;
	KodeboldsMath::Vector4 right;
	KodeboldsMath::Vector4 up;
};