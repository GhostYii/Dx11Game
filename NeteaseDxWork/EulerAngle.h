#pragma once
#include "Math.h"
#include "Matrix4x4.h"

class EulerAngle
{
public:
	EulerAngle() = default;
	EulerAngle(float roll, float pitch, float yaw)  { SetEuler(roll, pitch, yaw); }
	EulerAngle(const EulerAngle& other) { rotation = other.rotation; }

	~EulerAngle() = default;

public:
	float Roll() { return (float)atan2(rotation.value[3][2], sqrtf(1.f - pow(rotation.value[3][2], 2))); }
	float Pitch() { return  atan2(-rotation.value[3][1], rotation.value[3][3]); }
	float Yaw() { return atan2(-rotation.value[1][2], rotation.value[2][2]); }

	void SetEuler(float roll, float pitch, float yaw)
	{
		rotation.SetRotationZ(yaw);
		rotation.SetRotationY(pitch);
		rotation.SetRotationX(roll);
	}
	void SetEuler(const Vector3 euler)
	{
		rotation.SetRotationZ(euler.z);
		rotation.SetRotationY(euler.y);
		rotation.SetRotationX(euler.z);
	}
	// ·­¹ö½Ç
	void SetRoll(float roll) 
	{
		rotation.SetRotationX(roll); 
	}
	// ¸©Ñö½Ç
	void SetPitch(float pitch) 
	{ 
		rotation.SetRotationY(pitch);
	}
	// Æ«º½½Ç
	void SetYaw(float yaw) 
	{ 
		rotation.SetRotationZ(yaw);
	}

	Vector3 GetEulerAngle()
	{
		Vector3 euler;
		euler.x = Roll();
		euler.y = Pitch();
		euler.z = Yaw();
		return euler;
	}

	void SetIdenity() { rotation.SetIdentity(); }
	Matrix4x4 GetMatrix() { return rotation; }

private:
	Matrix4x4 rotation;
};
