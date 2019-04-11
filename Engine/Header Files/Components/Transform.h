#pragma once
#include <DirectXMath.h>
#include "Vector4.h"
#include "Matrix4.h"

struct Transform
{
	MathsHelper::Matrix4 transform;
	MathsHelper::Vector4 translation;
	MathsHelper::Vector4 rotation;
	MathsHelper::Vector4 scale;
	MathsHelper::Vector4 forward;
	MathsHelper::Vector4 right;
	MathsHelper::Vector4 up;
};