#pragma once
#include <cmath>
#include "Vector3.h"

namespace KodeboldsMath
{
	struct Vector4
	{
		float X;
		float Y;
		float Z;
		float W;

		//Structors
		Vector4();
		Vector4(const float x, const float y, const float z, const float w);
		~Vector4();

		//Accessors
		Vector3 XYZ() { return Vector3(X, Y, Z); };
		Vector2 XY() { return Vector2(X, Y); };
		Vector2 XZ() { return Vector2(X, Z); };
		Vector2 YZ() { return Vector2(Y, Z); };

		//Maths methods
		float Magnitude() const;
		Vector4& Clamp(float pMagnitude);
		Vector4& Normalise();

		//Operator overloads
		Vector4& operator+=(const Vector4& rhs);
		Vector4& operator-=(const Vector4& rhs);
		Vector4& operator*=(const float& rhs);
	};

	//Operator overloads
	inline Vector4 operator+(Vector4 lhs, const Vector4& rhs) { lhs += rhs; return lhs; }
	inline Vector4 operator-(Vector4 lhs, const Vector4& rhs) { lhs -= rhs;	return lhs; }
	inline Vector4 operator*(Vector4 lhs, const float& rhs) { lhs *= rhs;	return lhs; }
}