#pragma once
#include <math.h>
#include "Point.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;

const float ARC_TO_DEG = 57.29577951f;
const float DEG_TO_ARC = 0.01745329f;

template <typename T>
constexpr auto sq(const T& x)
{
	return x * x;
}

template<typename T>
T wrap_angle(T theta)
{
	const T modded = fmod(theta, (T)2.0 * (T)PI_D);
	return (modded > (T)PI_D) ?
		(modded - (T)2.0 * (T)PI_D) :
		modded;
}

template<typename T>
constexpr T interpolate(const T& src, const T& dst, float alpha)
{
	return src + (dst - src) * alpha;
}

template<typename T>
constexpr T to_rad(T deg)
{
	return deg * PI / (T)180.0;
}

template<typename T>
constexpr T to_deg(T rad)
{
	return rad * (T)180.0 / PI;
}