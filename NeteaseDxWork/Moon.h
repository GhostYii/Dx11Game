#pragma once
#include "ModelObject.h"

class Moon : public ModelObject
{
public:
	Moon() = default;
	virtual ~Moon() = default;

public:
	virtual void Start();
	virtual void Update(float frameTime);

public:
	Vector3 GetSpherePosition(const Vector3& center, float radius, float theta, float phi);
public:
	float selfRotSpeed = 0;
	float orbitSpeed = 0;
	float orbitHeight = 0;

	TransformPtr pTarget = nullptr;

private:
	float time = 0;
};

typedef std::shared_ptr<Moon> MoonPtr;