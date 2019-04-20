#pragma once
#include <cmath>
#include "Vector2.h"

namespace KodeboldsMath
{
	struct Vector3
	{
		float X;
		float Y;
		float Z;

		//Structors
		Vector3();
		Vector3(const float x, const float y, const float z);
		~Vector3();

		//Accessors
		Vector2 XY() { return Vector2(X, Y); };
		Vector2 XZ() { return Vector2(X, Z); };
		Vector2 YZ() { return Vector2(Y, Z); };

		//Maths methods
		float Magnitude() const;
		Vector3& Clamp(float pMagnitude);
		Vector3& Normalise();

		//Operator overloads
		Vector3& operator+=(const Vector3& rhs);
		Vector3& operator-=(const Vector3& rhs);
		Vector3& operator*=(const float& rhs);
	};

	//Operator overloads
	inline Vector3 operator+(Vector3 lhs, const Vector3& rhs) { lhs += rhs; return lhs; }
	inline Vector3 operator-(Vector3 lhs, const Vector3& rhs) { lhs -= rhs;	return lhs; }
	inline Vector3 operator*(Vector3 lhs, const float& rhs) { lhs *= rhs;	return lhs; }
}