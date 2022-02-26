#include "Spaceship.h"

void Spaceship::Start()
{
}

void Spaceship::Update(float frameTime)
{
	switch (state)
	{
	case Spaceship::SHIP_STATE_EARTH_ORBIT:
		break;
	case Spaceship::SHIP_STATE_MOON_ORBIT:
		break;
	case Spaceship::SHIP_STATE_TRANSFER:
		break;
	default:
		break;
	}

}

Vector3 Spaceship::GetSpherePosition(const Vector3& center, float radius, float theta, float phi)
{
	float sita = to_rad<float>(theta);
	Vector3 np = center;
	np.x += radius * sinf(theta) * cosf(phi);
	np.y += radius * cosf(theta);
	np.z += -(radius * sinf(theta) * sinf(phi));

	return np;
}
