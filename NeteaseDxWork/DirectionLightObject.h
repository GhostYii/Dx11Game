#pragma once
#include "Object.h"
#include "ComponentClass.h"
#include "types.h"

class DirectionLightObject : public Object
{
public:
	DirectionLightObject() = default;
	DirectionLightObject(float intensity, Color color) : intensity(intensity), color(color) {}
	~DirectionLightObject() = default;

public:
	float intensity = 1.f;
	Color color;
};

