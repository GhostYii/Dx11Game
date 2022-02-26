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
	virtual ~Object() = default;

public:
	TransformPtr pTransform;

public:
	virtual void Start() {}
	virtual void Update(int frameTime) {}
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
