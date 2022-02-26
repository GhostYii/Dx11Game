#pragma once
#include "ModelObject.h"

class Spaceship : public ModelObject
{
public:
	enum SHIP_STATE
	{
		SHIP_STATE_EARTH_ORBIT,
		SHIP_STATE_MOON_ORBIT,
		SHIP_STATE_TRANSFER
	};
public:
	Spaceship() = default;
	virtual ~Spaceship() = default;

public:
	virtual void Start();
	virtual void Update(float frameTime);

public:
	Vector3 GetSpherePosition(const Vector3& center, float radius, float theta, float phi);

public:
	SHIP_STATE state = SHIP_STATE_EARTH_ORBIT;

	float earthOribitHeight = 10.f;
	float moonOrbitHeight = 5.f;
	float flySpeed = 10.f;

	TransformPtr pEarthTrans = nullptr;
	TransformPtr pMoonTrans = nullptr;
};

typedef std::shared_ptr<Spaceship> SpaceshipPtr;