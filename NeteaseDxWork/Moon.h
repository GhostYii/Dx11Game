#pragma once
#include "ModelObject.h"

// 月球脚本
class Moon : public ModelObject
{
public:
	Moon() = default;
	virtual ~Moon() = default;

public:
	virtual void Start();
	virtual void Update(float frameTime);

private:
	/// <summary>
	/// 获取球面上的点坐标
	/// </summary>
	/// <param name="center">球心</param>
	/// <param name="radius">半径</param>
	/// <param name="theta">天顶角</param>
	/// <param name="phi">方位角</param>
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