#include "Spaceship.h"

void Spaceship::Start()
{
}

void Spaceship::Update(float frameTime)
{
	if (!pMoon || !pEarth)
		return;

	earthTransferPoint = pEarth->GetPosition() - (pEarth->GetPosition() - pMoon->GetPosition()).Normalized() * earthOribitHeight;
	moonTransferPoint = pMoon->GetPosition() + (pMoon->GetPosition() - pEarth->GetPosition()).Normalized() * moonOrbitHeight;

	Vector3 ellipsCenter = (earthTransferPoint + moonTransferPoint) / 2.f;
	float a = Vector3::Distance(earthTransferPoint, moonTransferPoint) / 2.f;

	switch (state)
	{
	case Spaceship::SHIP_STATE_EARTH_ORBIT:
		isGo = false;
		OnSphereOrbit(pEarth->GetPosition(), earthOribitHeight);
		break;
	case Spaceship::SHIP_STATE_MOON_ORBIT:
		isGo = false;
		OnSphereOrbit(pMoon->GetPosition(), moonOrbitHeight);
		break;
	case Spaceship::SHIP_STATE_TO_EARTH:
		tmpDis = Vector3::Distance(pTransform->GetPosition(), moonTransferPoint);
		if (!isGo)
			OnSphereOrbit(pMoon->GetPosition(), moonOrbitHeight);
		else
			OnEllipsoidOrbit(ellipsCenter, a, orbitRaduisZ, 0);
		if (Vector3::Distance(pTransform->GetPosition(), moonTransferPoint) <= 1.f)
			isGo = true;
		if (Vector3::Distance(pTransform->GetPosition(), earthTransferPoint) <= 1.f)
			state = SHIP_STATE_EARTH_ORBIT;
		break;
	case Spaceship::SHIP_STATE_TO_MOON:
		tmpDis = Vector3::Distance(pTransform->GetPosition(), earthTransferPoint);

		if (!isGo)
			OnSphereOrbit(pEarth->GetPosition(), earthOribitHeight);
		else
			OnEllipsoidOrbit(ellipsCenter, a, orbitRaduisZ, 0);
		if (Vector3::Distance(pTransform->GetPosition(), earthTransferPoint) <= 1.f)
			isGo = true;
		if (Vector3::Distance(pTransform->GetPosition(), moonTransferPoint) <= 1.f)
			state = SHIP_STATE_MOON_ORBIT;
		break;
	default:
		break;
	}

	time += frameTime * flySpeed;
}

Vector3 Spaceship::GetSpherePosition(const Vector3& center, float radius, float theta, float phi)
{
	Vector3 np = center;
	np.x += radius * sinf(theta) * cosf(phi);
	np.y += radius * cosf(theta);
	np.z += -(radius * sinf(theta) * sinf(phi));

	return np;
}

Vector3 Spaceship::GetEllipsoidPosition(const Vector3& center, float a, float b, float c, float theta, float phi)
{
	Vector3 np = center;
	np.x += a * sinf(theta) * cosf(phi);
	np.y += c * cosf(phi);
	np.z += -(b * sinf(theta) * sinf(phi));

	return np;
}

void Spaceship::OnSphereOrbit(const Vector3& center, float radius)
{
	constexpr float theta = to_rad<float>(90.f);
	Vector3 np = GetSpherePosition(center, radius, theta, time);
	pTransform->SetPosition(np);
}

void Spaceship::OnEllipsoidOrbit(const Vector3& center, float a, float b, float c)
{
	constexpr float theta = to_rad<float>(90.f);
	Vector3 np = GetEllipsoidPosition(center, a, b, c, theta, time);
	pTransform->SetPosition(np);
}
