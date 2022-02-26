#pragma once

class Vector3
{
public:
	Vector3() : x(0), y(0), z(0) {	}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {	}
	Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}

	~Vector3() = default;
public:
	Vector3 Normalized()
	{
		Vector3 normal;
		float sum = x + y + z;
		normal.x = x / sum;
		normal.y = y / sum;
		normal.z = z / sum;
		return normal;
	}

public:
	Vector3 operator+(const Vector3& other)
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	Vector3 operator-(const Vector3& other)
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}
	Vector3 operator*(float num)
	{
		return Vector3(x * num, y * num, z * num);
	}

public:
	static Vector3 Lerp(const Vector3& start, const Vector3& end, float delta)
	{
		Vector3 v;
		v.x = start.x * (1.f - delta) + end.x * delta;
		v.y = start.y * (1.f - delta) + end.y * delta;
		v.z = start.z * (1.f - delta) + end.z * delta;
		return v;
	}

	static float Distance(const Vector3& a, const Vector3& b)
	{
		return sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2) + powf(a.z - b.z, 2));
	}

public:
	float x, y, z;
};
