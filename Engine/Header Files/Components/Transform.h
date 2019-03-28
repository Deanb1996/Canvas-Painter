#pragma once
#include <DirectXMath.h>
#include "Vector4.h"
#include "Matrix4.h"

struct Transform
{
	MathsHelper::Matrix4 mTransform;
	MathsHelper::Vector4 mTranslation;
	MathsHelper::Vector4 mRotation;
	MathsHelper::Vector4 mScale;
	MathsHelper::Vector4 mForward;
	MathsHelper::Vector4 mRight;
	MathsHelper::Vector4 mUp;
};