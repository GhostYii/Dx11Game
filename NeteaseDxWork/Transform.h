#pragma once
//#include "EulerAngle.h"

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
	Matrix4x4 CalcMat()
	{
		this->matrix.SetIdentity();
		this->matrix.SetScale(scale);
		this->matrix.SetEuler(eulerAngle.x, eulerAngle.y, eulerAngle.z);
		this->matrix.SetTranslation(position);

		return this->matrix;
	}

private:
	Vector3 position;
	Vector3 eulerAngle;
	Vector3 scale;

	Matrix4x4 matrix;
};