#pragma once
#include "Vector3.h"

#include <math.h>
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

	void SetTranslation(const Vector3& translation)
	{
		//SetIdentity();
		value[3][0] = translation.x;
		value[3][1] = translation.y;
		value[3][2] = translation.z;
	}

	void SetScale(const Vector3& scale)
	{
		//SetIdentity();
		value[0][0] = scale.x;
		value[1][1] = scale.y;
		value[2][2] = scale.z;
	}

	void SetEuler(float roll, float pitch, float yaw)
	{
		// z-x-y
		SetRotationZ(yaw);		
		SetRotationX(roll);
		SetRotationY(pitch);
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

	Vector3 GetEulerAngle()
	{
		Vector3 euler;
		euler.x = Roll();
		euler.y = Pitch();
		euler.z = Yaw();
		return euler;
	}
	float Roll() { return (float)atan2(value[3][2], sqrtf(1.f - powf(value[3][2], 2))); }
	float Pitch() { return  atan2(-value[3][1], value[3][3]); }
	float Yaw() { return atan2(-value[1][2], value[2][2]); }

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

	void SetPerpectiveFovLH(float fov, float aspect, float zNear, float zFar)
	{
		float yScale = 1.f / tan(fov / 2.f);
		float xScale = yScale / aspect;		

		value[0][0] = xScale;
		value[1][1] = yScale;
		value[2][2] = zFar / (zFar - zNear);
		value[2][3] = 1.f;
		value[3][2] = (-zNear * zFar) / (zFar - zNear);
	}

	void SetOrthoLH(float width, float height, float nearPlane, float farPlane)
	{
		SetIdentity();
		value[0][0] = 2.0f / width;
		value[1][1] = 2.0f / height;
		value[2][2] = 1.0f / (farPlane - nearPlane);
		value[3][2] = -(nearPlane / (farPlane - nearPlane));
	}

	float GetDeterminant()
	{
		Vector4 minor, v1, v2, v3;
		float det;

		v1 = Vector4(this->value[0][0], this->value[1][0], this->value[2][0], this->value[3][0]);
		v2 = Vector4(this->value[0][1], this->value[1][1], this->value[2][1], this->value[3][1]);
		v3 = Vector4(this->value[0][2], this->value[1][2], this->value[2][2], this->value[3][2]);


		minor.Cross(v1, v2, v3);
		det = -(this->value[0][3] * minor.x + this->value[1][3] * minor.y + this->value[2][3] * minor.z +
			this->value[3][3] * minor.w);
		return det;
	}

	void Inverse()
	{
		int a, i, j;
		Matrix4x4 out;
		Vector4 v, vec[3];
		float det = 0.0f;

		det = this->GetDeterminant();
		if (!det) return;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (j != i)
				{
					a = j;
					if (j > i) a = a - 1;
					vec[a].x = (this->value[j][0]);
					vec[a].y = (this->value[j][1]);
					vec[a].z = (this->value[j][2]);
					vec[a].w = (this->value[j][3]);
				}
			}
			v.Cross(vec[0], vec[1], vec[2]);

			out.value[0][i] = pow(-1.0f, i) * v.x / det;
			out.value[1][i] = pow(-1.0f, i) * v.y / det;
			out.value[2][i] = pow(-1.0f, i) * v.z / det;
			out.value[3][i] = pow(-1.0f, i) * v.w / det;
		}

		this->SetMatrix(out);
	}

	Vector3 GetDirectionX()
	{
		return Vector3(value[0][0], value[0][1], value[0][2]);
	}
	Vector3 GetDirectionZ()
	{
		return Vector3(value[2][0], value[2][1], value[2][2]);
	}

	Vector3 GetTranslation()
	{
		return Vector3(value[3][0], value[3][1], value[3][2]);
	}

public:
	float value[4][4] = { 0 };

};
