#pragma once
#include "Math.h"

class Transform
{
public:
	Transform() : position(), eulerAngle(), scale(1, 1, 1)
	{
		matrix.SetIdentity();
		CalcMat();
	}
	Transform(const Vector3 position, const Vector3 euler, const Vector3 scale)
	{
		this->position = position;
		this->eulerAngle = euler;
		this->scale = scale;
		CalcMat();
	}
	Transform(const Transform& other) : position(other.position), eulerAngle(other.eulerAngle), scale(other.scale)
	{
		CalcMat();
	}
	~Transform() = default;

public:
	Vector3 GetPosition()
	{
		return position;
	}
	Vector3 GetEulerAngle()
	{
		return eulerAngle;
	}
	Vector3 GetScale()
	{
		return scale;
	}

	Matrix4x4 GetMatrix() { return matrix; }

	void SetPosition(const Vector3 position)
	{
		this->position = position;
		CalcMat();
	}
	void SetRotation(const Vector3 euler)
	{
		this->eulerAngle = euler;
		CalcMat();
	}
	void SetScale(const Vector3 scale)
	{
		this->scale = scale;
		CalcMat();
	}

private:
	void CalcMat()
	{
		matrix.SetIdentity();				
		Matrix4x4 tmp;

		tmp.SetIdentity();
		tmp.SetScale(scale);
		matrix *= tmp;

		tmp.SetIdentity();
		tmp.SetRotationX(to_rad<float>(eulerAngle.x));
		matrix *= tmp;

		tmp.SetIdentity();
		tmp.SetRotationY(to_rad<float>(eulerAngle.y));
		matrix *= tmp;

		tmp.SetIdentity();
		tmp.SetRotationZ(to_rad<float>(eulerAngle.z));
		matrix *= tmp;

		tmp.SetIdentity();
		tmp.SetTranslation(position);
		matrix *= tmp;
	}

private:
	Vector3 position;
	Vector3 eulerAngle;
	Vector3 scale;

	Matrix4x4 matrix;
};