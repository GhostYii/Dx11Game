#pragma once
#include "CameraObject.h"

class TPCameraObject : public CameraObject
{
public:
	TPCameraObject(float aspect) : CameraObject(aspect) {}
	virtual ~TPCameraObject() = default;

public:
	virtual void Update(float frameTime);

public:
	TransformPtr pTarget = nullptr;
	float distance = -5.f;

	float mouseSensitivity = 8.f;

};

