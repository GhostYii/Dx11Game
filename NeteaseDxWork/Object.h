#pragma once
#include "ComponentClass.h"
#include "Transform.h"

class Object
{
public:
	Object()
	{
		pTransform = std::make_shared<Transform>();
	};
	~Object() = default;

public:
	TransformPtr pTransform;

public:
	Vector3 Forward()
	{
		return pTransform->GetMatrix().GetDirectionZ();
	}
	Vector3 Right()
	{
		return pTransform->GetMatrix().GetDirectionX();
	}
};
