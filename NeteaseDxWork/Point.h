#pragma once
#include <windef.h>

class Point
{
public:
	Point() :x(0), y(0) {}
	Point(int x, int y) : x(x), y(y) {}
	Point(const Point& other) : x(other.x), y(other.y) {}

	~Point() = default;

public:
	bool operator==(const Point& other)
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const Point& other)
	{
		return x != other.x || y != other.y;
	}
	bool operator!=(const POINT& other)
	{
		return x != other.x || y != other.y;
	}

public:
	int x = 0, y = 0;
};