#pragma once
#include "Matrix4.h"
#include <cmath>

namespace MathsHelper
{
	const double PI = 3.141592653589793238463;

	/// <summary>
	/// Creates an identity matrix
	/// </summary>
	/// <returns>Returns an identity matrix</returns>
	static Matrix4 Identity()
	{
		return Matrix4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}

	/// <summary>
	/// Creates a rotation matrix around the y axis with a given angle in radians
	/// </summary>
	/// <param name="pAngle">Given angle of rotation</param>
	/// <returns>Rotation matrix for a rotation around the x axis</returns>
	static Matrix4 RotationMatrixX(const float& pAngle)
	{
		//Sin and cosin of angle
		float s = sin(pAngle);
		float c = cos(pAngle);

		Matrix4 matrix(
			1, 0, 0, 0,
			0, c, s, 0,
			0, -s, c, 0,
			0, 0, 0, 1);

		return matrix;
	}

	/// <summary>
	/// Creates a rotation matrix around the y axis with a given angle in radians
	/// </summary>
	/// <param name="pAngle">Given angle of rotation</param>
	/// <returns>Rotation matrix for a rotation around the y axis</returns>
	static Matrix4 RotationMatrixY(const float& pAngle)
	{
		//Sin and cosin of angle
		float s = sin(pAngle);
		float c = cos(pAngle);

		Matrix4 matrix(
			c, 0, -s, 0,
			0, 1, 0, 0,
			s, 0, c, 0,
			0, 0, 0, 1);

		return matrix;
	}

	/// <summary>
	/// Creates a rotation matrix around the z axis with a given angle in radians
	/// </summary>
	/// <param name="pAngle">Given angle of rotation</param>
	/// <returns>Rotation matrix for a rotation around the z axis</returns>
	static Matrix4 RotationMatrixZ(const float& pAngle)
	{
		//Sin and cosin of angle
		float s = sin(pAngle);
		float c = cos(pAngle);

		Matrix4 matrix(
			c, s, 0, 0,
			-s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		return matrix;
	}

	/// <summary>
	/// Creates a rotation matrix around a given axis with a given angle in radians
	/// </summary>
	/// <param name="pAngle">Given angle of rotation</param>
	/// <param name="pAxis">Given axis to rotation around</param>
	/// <returns>Rotation matrix for a rotation around the given axis</returns>
	static Matrix4 RotationMatrixAxis(const float& pAngle, const Vector4& pAxis)
	{
		//Sin and cosin of angle
		const float s = sin(pAngle);
		const float c = cos(pAngle);
		const float oneSubC = 1 - c;

		//Squares of x, y and z components of axis
		const float xSqrd = pAxis.X * pAxis.X;
		const float ySqrd = pAxis.Y * pAxis.Y;
		const float zSqrd = pAxis.Z * pAxis.Z;

		//X, y and z components multiplied by sin
		const float xS = pAxis.X * s;
		const float yS = pAxis.Y * s;
		const float zS = pAxis.Z * s;

		//X, y and z components multiplied by each other component
		const float xY = pAxis.X * pAxis.Y;
		const float xZ = pAxis.X * pAxis.Z;
		const float yZ = pAxis.Y * pAxis.Z;

		Matrix4 matrix(
			//Column 1             // Column 2            //Column 3             //Column 4
			(c + xSqrd) * oneSubC, (xY * oneSubC) - zS,   (xZ * oneSubC) + yS,   0,  //Row 1
			(xY * oneSubC) + zS,   (c + ySqrd) * oneSubC, (yZ * oneSubC) - xS,   0,  //Row 2
			(xZ * oneSubC) - yS,   (yZ * oneSubC) + xS,   (c + zSqrd) * oneSubC, 0,  //Row 3
			0, 0, 0, 1); //Row 4

		return matrix;
	}

	/// <summary>
	/// Creates a translation matrix from a given vector
	/// </summary>
	/// <param name="pVector">Given vector containing x, y, z components of translation</param>
	/// <returns>Translation matrix containing the given translation vector</returns>
	static Matrix4 TranslationMatrix(const Vector4& pVector)
	{
		Matrix4 matrix(
			1, 0, 0, pVector.X,
			0, 1, 0, pVector.Y,
			0, 0, 1, pVector.Z,
			0, 0, 0, 1);

		return matrix;
	}

	/// <summary>
	/// Creates a scale matrix from a given vector
	/// </summary>
	/// <param name="pVector">Given vector containing x, y, z components of scale</param>
	/// <returns>Scale matrix containing the given scale vector</returns>
	static Matrix4 ScaleMatrix(const Vector4& pVector)
	{
		Matrix4 matrix(
			pVector.X, 0, 0, 0,
			0, pVector.Y, 0, 0,
			0, 0, pVector.Z, 0,
			0, 0, 0, 1);

		return matrix;
	}

	/// <summary>
	/// Calculates the transpose of a given matrix
	/// </summary>
	/// <param name="pMatrix">Given matrix to transpose</param>
	/// <returns>Transposed matrix of the given matrix</returns>
	static Matrix4 Transpose(const Matrix4& pMatrix)
	{
		Matrix4 matrix(
			pMatrix._11, pMatrix._21, pMatrix._31, pMatrix._41,
			pMatrix._12, pMatrix._21, pMatrix._32, pMatrix._42,
			pMatrix._13, pMatrix._23, pMatrix._33, pMatrix._43,
			pMatrix._14, pMatrix._24, pMatrix._34, pMatrix._44);

		return matrix;
	}

	/// <summary>
	///  Converts given angle from degrees to radians
	/// </summary>
	/// <returns>Returns given angle in radians</returns>
	static float DegreesToRadians(const float& pAngle)
	{
		return (pAngle * PI) / 180;
	}

	/// <summary>
	/// Converts given angle from radians to degrees
	/// </summary>
	/// <returns>REturns given angle in degrees</returns>
	static float RadiansToDegrees(const float& pAngle)
	{
		return (pAngle * 180) / PI;
	}
}