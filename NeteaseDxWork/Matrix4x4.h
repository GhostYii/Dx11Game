#pragma once
#include "Vector3.h"

#include <memory>

class Matrix4x4
{
public:
	Matrix4x4() = default;
	~Matrix4x4() = default;

public:
	void SetMatrix(const Matrix4x4& matrix)
	{
		memcpy(value, matrix.value, sizeof(float) * 16);
	}

	void SetIdentity()
	{
		memset(value, 0, sizeof(float) * 16);
		for (int i = 0; i < 4; i++)
			value[i][i] = 1;
	}

	void SetTranslate(const Vector3& translation)
	{
		SetIdentity();
		value[3][0] = translation.x;
		value[3][1] = translation.y;
		value[3][2] = translation.z;
	}

	void SetScale(const Vector3& scale)
	{
		SetIdentity();
		value[0][0] = scale.x;
		value[1][1] = scale.y;
		value[2][2] = scale.z;
	}

	void SetRotationX(float x)
	{
		value[1][1] = cos(x);
		value[1][2] = sin(x);
		value[2][1] = -sin(x);
		value[2][2] = cos(x);
	}

	void SetRotationY(float y)
	{
		value[0][0] = cos(y);
		value[0][2] = -sin(y);
		value[2][0] = sin(y);
		value[2][2] = cos(y);
	}

	void SetRotationZ(float z)
	{
		value[0][0] = cos(z);
		value[0][1] = sin(z);
		value[1][0] = -sin(z);
		value[1][1] = cos(z);
	}

	void operator *=(const Matrix4x4& matrix)
	{
		Matrix4x4 out;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				out.value[i][j] =
					value[i][0] * matrix.value[0][j] + value[i][1] * matrix.value[1][j] +
					value[i][2] * matrix.value[2][j] + value[i][3] * matrix.value[3][j];
			}
		}
		SetMatrix(out);
	}

	void SetOrthoLH(float width, float height, float nearPlane, float farPlane)
	{
		SetIdentity();
		value[0][0] = 2.0f / width;
		value[1][1] = 2.0f / height;
		value[2][2] = 1.0f / (farPlane - nearPlane);
		value[3][2] = -(nearPlane / (farPlane - nearPlane));
	}



public:
	float value[4][4] = { 0 };

};
