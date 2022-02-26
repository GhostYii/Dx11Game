#include "Earth.h"

void Earth::Start()
{
}

void Earth::Update(float frameTime)
{
	Vector3 euler = pTransform->GetEulerAngle();
	euler.y = pTransform->GetEulerAngle().y + frameTime * rotateSpeed;
	pTransform->SetRotation(euler);
}
