#include "TPCameraObject.h"

void TPCameraObject::Update(float frameTime)
{
	if (!pTarget)
		return;

	Vector3 newPos = pTarget->GetPosition() + Forward() * distance;

	pTransform->SetPosition(newPos);	
	UpdateVP(aspect);
}
