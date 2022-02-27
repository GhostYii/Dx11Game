#pragma once
#include "ModelObject.h"

// ����ű�
class Earth : public ModelObject
{
public:
	Earth() = default;
	virtual ~Earth() = default;

public:
	virtual void Start();
	virtual void Update(float frameTime);

public:
	float rotateSpeed = 0;
};

typedef std::shared_ptr<Earth> EarthPtr;