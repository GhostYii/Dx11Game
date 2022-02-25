#pragma once
#include "Vector2.h"
#include "Vector4.h"
#include "Matrix4x4.h"

struct Color
{
	float r;
	float g;
	float b;
	//float a;
};

struct Vertex
{
	Vector3 position;
	Vector2 texcoord;
};

__declspec(align(16))
struct Constant
{
	Matrix4x4 model;
	Matrix4x4 view;
	Matrix4x4 projection;
	Vector4 light;
	Vector4 cameraPosition;
	float lightIntensity;
	Color lightColor;
};

