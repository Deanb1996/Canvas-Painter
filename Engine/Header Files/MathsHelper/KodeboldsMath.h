#pragma once
#include "Matrix4.h"

namespace KodeboldsMath
{
	const float PI = 3.141592653589793238463f;

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
			//Column 1             //Column 2            //Column 3             //Column 4
			(c + xSqrd) * oneSubC, (xY * oneSubC) - zS, (xZ * oneSubC) + yS, 0,  //Row 1
			(xY * oneSubC) + zS, (c + ySqrd) * oneSubC, (yZ * oneSubC) - xS, 0,  //Row 2
			(xZ * oneSubC) - yS, (yZ * oneSubC) + xS, (c + zSqrd) * oneSubC, 0,  //Row 3
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
	/// Calculates the inverse of a given matrix using the Minors, Cofactors and Adjugate method
	/// </summary>
	/// <param name="pMatrix">Matrix to be inverted</param>
	/// <returns>Inverse of the given matrix</returns>
	static Matrix4 Inverse(const Matrix4& pMatrix)
	{
		//Calculate determinant
		float det =
			pMatrix._11 * pMatrix._22 * pMatrix._33 * pMatrix._44 +
			pMatrix._11 * pMatrix._23 * pMatrix._34 * pMatrix._42 +
			pMatrix._11 * pMatrix._24 * pMatrix._32 * pMatrix._43 +
			pMatrix._12 * pMatrix._21 * pMatrix._34 * pMatrix._43 +
			pMatrix._12 * pMatrix._23 * pMatrix._31 * pMatrix._44 +
			pMatrix._12 * pMatrix._24 * pMatrix._33 * pMatrix._41 +
			pMatrix._13 * pMatrix._21 * pMatrix._32 * pMatrix._44 +
			pMatrix._13 * pMatrix._22 * pMatrix._34 * pMatrix._41 +
			pMatrix._13 * pMatrix._24 * pMatrix._31 * pMatrix._42 +
			pMatrix._14 * pMatrix._21 * pMatrix._33 * pMatrix._42 +
			pMatrix._14 * pMatrix._22 * pMatrix._31 * pMatrix._43 +
			pMatrix._14 * pMatrix._23 * pMatrix._32 * pMatrix._41 -
			pMatrix._11 * pMatrix._22 * pMatrix._34 * pMatrix._43 -
			pMatrix._11 * pMatrix._23 * pMatrix._32 * pMatrix._44 -
			pMatrix._11 * pMatrix._24 * pMatrix._33 * pMatrix._42 -
			pMatrix._12 * pMatrix._21 * pMatrix._33 * pMatrix._44 -
			pMatrix._12 * pMatrix._23 * pMatrix._34 * pMatrix._41 -
			pMatrix._12 * pMatrix._24 * pMatrix._31 * pMatrix._43 -
			pMatrix._13 * pMatrix._21 * pMatrix._34 * pMatrix._42 -
			pMatrix._13 * pMatrix._22 * pMatrix._31 * pMatrix._44 -
			pMatrix._13 * pMatrix._24 * pMatrix._32 * pMatrix._41 -
			pMatrix._14 * pMatrix._21 * pMatrix._32 * pMatrix._43 -
			pMatrix._14 * pMatrix._22 * pMatrix._33 * pMatrix._41 -
			pMatrix._14 * pMatrix._23 * pMatrix._31 * pMatrix._42;

		//If determinant is not 0, then an inverse of the matrix exists
		if (det != 0)
		{
			Matrix4 matrix;

			//Row 1 Column 1
			matrix._11 =
				pMatrix._22 * pMatrix._33 * pMatrix._44 +
				pMatrix._23 * pMatrix._34 * pMatrix._42 +
				pMatrix._24 * pMatrix._32 * pMatrix._43 -
				pMatrix._22 * pMatrix._34 * pMatrix._43 -
				pMatrix._23 * pMatrix._32 * pMatrix._44 -
				pMatrix._24 * pMatrix._33 * pMatrix._42;

			//Row 1 Column 2
			matrix._12 =
				pMatrix._12 * pMatrix._33 * pMatrix._43 +
				pMatrix._13 * pMatrix._32 * pMatrix._44 +
				pMatrix._14 * pMatrix._33 * pMatrix._42 -
				pMatrix._12 * pMatrix._33 * pMatrix._44 -
				pMatrix._13 * pMatrix._34 * pMatrix._42 -
				pMatrix._14 * pMatrix._32 * pMatrix._43;

			//Row 1 Column 3
			matrix._13 =
				pMatrix._12 * pMatrix._23 * pMatrix._44 +
				pMatrix._13 * pMatrix._24 * pMatrix._42 +
				pMatrix._14 * pMatrix._22 * pMatrix._43 -
				pMatrix._12 * pMatrix._24 * pMatrix._43 -
				pMatrix._13 * pMatrix._22 * pMatrix._44 -
				pMatrix._14 * pMatrix._23 * pMatrix._42;

			//Row 1 Column 4
			matrix._14 =
				pMatrix._12 * pMatrix._24 * pMatrix._33 +
				pMatrix._13 * pMatrix._22 * pMatrix._34 +
				pMatrix._14 * pMatrix._23 * pMatrix._32 -
				pMatrix._12 * pMatrix._23 * pMatrix._34 -
				pMatrix._13 * pMatrix._24 * pMatrix._32 -
				pMatrix._14 * pMatrix._22 * pMatrix._33;

			//Row 2 Column 1
			matrix._21 =
				pMatrix._21 * pMatrix._34 * pMatrix._43 +
				pMatrix._23 * pMatrix._31 * pMatrix._44 +
				pMatrix._24 * pMatrix._33 * pMatrix._41 -
				pMatrix._21 * pMatrix._33 * pMatrix._44 -
				pMatrix._23 * pMatrix._34 * pMatrix._41 -
				pMatrix._24 * pMatrix._31 * pMatrix._43;

			//Row 2 Column 2
			matrix._22 =
				pMatrix._11 * pMatrix._33 * pMatrix._44 +
				pMatrix._13 * pMatrix._34 * pMatrix._41 +
				pMatrix._14 * pMatrix._31 * pMatrix._43 -
				pMatrix._11 * pMatrix._34 * pMatrix._43 -
				pMatrix._13 * pMatrix._31 * pMatrix._44 -
				pMatrix._14 * pMatrix._33 * pMatrix._41;

			//Row 2 Column 3
			matrix._23 =
				pMatrix._11 * pMatrix._24 * pMatrix._43 +
				pMatrix._13 * pMatrix._21 * pMatrix._44 +
				pMatrix._14 * pMatrix._23 * pMatrix._41 -
				pMatrix._11 * pMatrix._23 * pMatrix._44 -
				pMatrix._13 * pMatrix._24 * pMatrix._41 -
				pMatrix._14 * pMatrix._21 * pMatrix._43;

			//Row 2 Column 4
			matrix._24 =
				pMatrix._11 * pMatrix._23 * pMatrix._34 +
				pMatrix._13 * pMatrix._24 * pMatrix._31 +
				pMatrix._14 * pMatrix._21 * pMatrix._33 -
				pMatrix._11 * pMatrix._24 * pMatrix._33 -
				pMatrix._13 * pMatrix._21 * pMatrix._34 -
				pMatrix._14 * pMatrix._23 * pMatrix._31;

			//Row 3 Column 1
			matrix._31 =
				pMatrix._21 * pMatrix._32 * pMatrix._44 +
				pMatrix._22 * pMatrix._34 * pMatrix._41 +
				pMatrix._24 * pMatrix._31 * pMatrix._42 -
				pMatrix._21 * pMatrix._34 * pMatrix._42 -
				pMatrix._22 * pMatrix._31 * pMatrix._44 -
				pMatrix._24 * pMatrix._32 * pMatrix._41;

			//Row 3 Column 2
			matrix._32 =
				pMatrix._11 * pMatrix._34 * pMatrix._42 +
				pMatrix._12 * pMatrix._31 * pMatrix._44 +
				pMatrix._14 * pMatrix._32 * pMatrix._41 -
				pMatrix._11 * pMatrix._32 * pMatrix._44 -
				pMatrix._12 * pMatrix._34 * pMatrix._41 -
				pMatrix._14 * pMatrix._31 * pMatrix._42;

			//Row 3 Column 3
			matrix._33 =
				pMatrix._11 * pMatrix._22 * pMatrix._44 +
				pMatrix._12 * pMatrix._24 * pMatrix._41 +
				pMatrix._14 * pMatrix._21 * pMatrix._42 -
				pMatrix._11 * pMatrix._24 * pMatrix._42 -
				pMatrix._12 * pMatrix._21 * pMatrix._44 -
				pMatrix._14 * pMatrix._22 * pMatrix._41;

			//Row 3 Column 4
			matrix._34 =
				pMatrix._11 * pMatrix._24 * pMatrix._32 +
				pMatrix._12 * pMatrix._21 * pMatrix._34 +
				pMatrix._14 * pMatrix._22 * pMatrix._31 -
				pMatrix._11 * pMatrix._22 * pMatrix._34 -
				pMatrix._12 * pMatrix._24 * pMatrix._31 -
				pMatrix._14 * pMatrix._21 * pMatrix._32;

			//Row 4 Column 1
			matrix._41 =
				pMatrix._21 * pMatrix._33 * pMatrix._42 +
				pMatrix._22 * pMatrix._31 * pMatrix._43 +
				pMatrix._23 * pMatrix._32 * pMatrix._41 -
				pMatrix._21 * pMatrix._32 * pMatrix._43 -
				pMatrix._22 * pMatrix._33 * pMatrix._41 -
				pMatrix._23 * pMatrix._31 * pMatrix._42;

			//Row 4 Column 2
			matrix._42 =
				pMatrix._11 * pMatrix._32 * pMatrix._43 +
				pMatrix._12 * pMatrix._33 * pMatrix._41 +
				pMatrix._13 * pMatrix._31 * pMatrix._42 -
				pMatrix._11 * pMatrix._33 * pMatrix._42 -
				pMatrix._12 * pMatrix._31 * pMatrix._43 -
				pMatrix._13 * pMatrix._32 * pMatrix._41;

			//Row 4 Column 3
			matrix._43 =
				pMatrix._11 * pMatrix._23 * pMatrix._42 +
				pMatrix._12 * pMatrix._21 * pMatrix._43 +
				pMatrix._13 * pMatrix._22 * pMatrix._41 -
				pMatrix._11 * pMatrix._22 * pMatrix._43 -
				pMatrix._12 * pMatrix._23 * pMatrix._41 -
				pMatrix._13 * pMatrix._21 * pMatrix._42;

			//Row 4 Column 4
			matrix._44 =
				pMatrix._11 * pMatrix._22 * pMatrix._33 +
				pMatrix._12 * pMatrix._23 * pMatrix._31 +
				pMatrix._13 * pMatrix._21 * pMatrix._32 -
				pMatrix._11 * pMatrix._23 * pMatrix._32 -
				pMatrix._12 * pMatrix._21 * pMatrix._33 -
				pMatrix._13 * pMatrix._22 * pMatrix._31;

			//Calculate 1/determinant and multiply each element of the matrix by it
			det = 1.0f / det;
			for (int i = 0; i < 16; i++)
			{
				matrix.mElements[i] = matrix.mElements[i] * det;
			}

			return matrix;
		}
		return pMatrix;
	}

	/// <summary>
	/// Calculates and returns the dot product between two given vectors
	/// </summary>
	/// <param name="pVectorA">Left hand side vector of the dot product</param>
	/// <param name="pVectorB">Right hand side vector of the dot product</param>
	/// <returns>Dot product of the two given vectors</returns>
	static float Dot(const Vector4& pVectorA, const Vector4& pVectorB)
	{
		return (pVectorA.X * pVectorB.X) + (pVectorA.Y * pVectorB.Y) + (pVectorA.Z * pVectorB.Z) + (pVectorA.W * pVectorB.W);
	}

	/// <summary>
	/// Calculates the cross product of two given vectors
	/// </summary>
	/// <param name="pVectorA">Left hand side vector of the cross product</param>
	/// <param name="pVectorB">Right hand side vector of the cross product </param>
	/// <returns>Cross product of the two given vectors</returns>
	static Vector4 Cross(const Vector4& pVectorA, const Vector4& pVectorB)
	{
		Vector4 vector(
			(pVectorA.Y * pVectorB.Z) - (pVectorA.Z * pVectorB.Y),
			(pVectorA.Z * pVectorB.X) - (pVectorA.X * pVectorB.Z),
			(pVectorA.X * pVectorB.Y) - (pVectorA.Y * pVectorB.X),
			1);

		return vector;
	}

	/// <summary>
	/// Multiplies a given vector by a given matrix
	/// </summary>
	/// <param name="pVector">Given vector to multiply</param>
	/// <param name="pMatrix">Given matrix to multiply by</param>
	/// <returns>Vector containing result of multiplication</returns>
	static Vector4 MultiplyVectorMatrix(const Vector4& pVector, const Matrix4& pMatrix)
	{
		Vector4 vector(
			pMatrix._11 * pVector.X + pMatrix._12 * pVector.Y + pMatrix._13 * pVector.Z + pMatrix._14 * pVector.W,
			pMatrix._21 * pVector.X + pMatrix._22 * pVector.Y + pMatrix._23 * pVector.Z + pMatrix._24 * pVector.W,
			pMatrix._31 * pVector.X + pMatrix._32 * pVector.Y + pMatrix._33 * pVector.Z + pMatrix._34 * pVector.W,
			pMatrix._41 * pVector.X + pMatrix._42 * pVector.Y + pMatrix._43 * pVector.Z + pMatrix._44 * pVector.W
		);

		return vector;
	}

	/// <summary>
	/// Calculates a look at matrix with the given eye position, look at and up direction vectors
	/// </summary>
	/// <param name="pEyePosition">Given vector containing the eye position</param>
	/// <param name="pLookAt">Given vector containing the look at direction</param>
	/// <param name="pUp">Given vector containing the up direction</param>
	/// <returns>Matrix containing the result of the calculation</returns>
	static Matrix4 LookAt(const Vector4& pEyePosition, const Vector4& pLookAt, const Vector4& pUp)
	{
		const Vector4 zAxis = Vector4(pLookAt - pEyePosition).Normalise();
		const Vector4 xAxis = Cross(pUp, zAxis).Normalise();
		const Vector4 yAxis = Cross(zAxis, xAxis);

		Matrix4 matrix(
			//Column 1                 //Column 2                 //Column 3                 //Column 4
			xAxis.X,				   yAxis.X,                   zAxis.X,                   0,  //Row 1
			xAxis.Y,                   yAxis.Y,                   zAxis.Y,                   0,  //Row 2
			xAxis.Z,                   yAxis.Z,                   zAxis.Z,                   0,  //Row 3
			-Dot(xAxis, pEyePosition), -Dot(yAxis, pEyePosition), -Dot(zAxis, pEyePosition), 1); //Row 4

		return matrix;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="pFOV"></param>
	/// <param name="pAspectRatio"></param>
	/// <param name="pNear"></param>
	/// <param name="pFar"></param>
	/// <returns></returns>
	static Matrix4 Projection(const float& pFOV, const float& pAspectRatio, const float& pNear, const float& pFar)
	{
		if (pFOV <= 0 || pAspectRatio == 0)
		{
			return Matrix4();
		}

		const float frustrumDepth = pFar - pNear;
		const float oneDivDepth = 1 / frustrumDepth;

		Matrix4 matrix;
		matrix._22 = 1 / tan(0.5f * pFOV);
		matrix._11 = matrix._22 / pAspectRatio;
		matrix._33 = pFar * oneDivDepth;
		matrix._43 = (-pFar * pNear) * oneDivDepth;
		matrix._34 = 1;
		matrix._44 = 0;

		return matrix;
	}

	/// <summary>
	///  Converts given angle from degrees to radians
	/// </summary>
	/// <returns>Returns given angle in radians</returns>
	static float DegreesToRadians(const float& pAngle)
	{
		return (pAngle * PI) / 180.0f;
	}

	/// <summary>
	/// Converts given angle from radians to degrees
	/// </summary>
	/// <returns>Returns given angle in degrees</returns>
	static float RadiansToDegrees(const float& pAngle)
	{
		return (pAngle * 180.0f) / PI;
	}
}