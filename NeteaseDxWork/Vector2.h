#pragma once

class Vector2
{
public:
	Vector2() : x(0), y(0) {}
	Vector2(float x, float y) : x(x), y(y) {}
	Vector2(const Vector2& other) : x(other.x), y(other.y) {}
	~Vector2() = default;

public:
	Vector2& operator+(const Vector2& other)
	{
		Vector2 v;
		v.x = x + other.x;
		v.y = y + other.y;
		return v;
	}

	Vector2& operator-(const Vector2& other)
	{
		Vector2 v;
		v.x = x - other.x;
		v.y = y - other.y;
		return v;
	}

public:
	float x = 0, y = 0;
};