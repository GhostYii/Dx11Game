#include "Moon.h"

void Moon::Start()
{
}

void Moon::Update(float frameTime)
{
	Vector3 euler = pTransform->GetEulerAngle();
	euler.y = pTransform->GetEulerAngle().y + frameTime * selfRotSpeed;
	pTransform->SetRotation(euler);

	if (!pTarget)
		return;

	constexpr float theta = to_rad<float>(90.f);
	Vector3 np = GetSpherePosition(pTarget->GetPosition(), orbitHeight, theta, time);
	pTransform->SetPosition(np);

	//asinf()

	time += frameTime * orbitSpeed;
}

Vector3 Moon::GetSpherePosition(const Vector3& center, float radius, float theta, float phi)
{
	float sita = to_rad<float>(theta);
	Vector3 np = center;
	np.x += radius * sinf(theta) * cosf(phi);
	np.y += radius * cosf(theta);
	np.z += -(radius * sinf(theta) * sinf(phi));

	return np;
}

