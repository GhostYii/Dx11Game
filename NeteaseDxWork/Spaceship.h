#pragma once
#include "ModelObject.h"

// 飞船脚本
class Spaceship : public ModelObject
{
public:
	enum SHIP_STATE
	{
		SHIP_STATE_EARTH_ORBIT,		// 在地球轨道运行
		SHIP_STATE_MOON_ORBIT,		// 在月球轨道运行
		SHIP_STATE_TO_EARTH,		// 月地转移
		SHIP_STATE_TO_MOON			// 地月转移
	};
public:
	Spaceship() = default;
	virtual ~Spaceship() = default;

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
	/// <summary>
	/// 获取椭球上的点坐标
	/// </summary>
	/// <param name="center">椭球原点</param>
	/// <param name="a">赤道半径（x方向）</param>
	/// <param name="b">赤道半径（z方向）</param>
	/// <param name="c">极半径（y方向）</param>
	/// <param name="theta">天顶角</param>
	/// <param name="phi">方位角</param>
	Vector3 GetEllipsoidPosition(const Vector3& center, float a, float b, float c, float theta, float phi);

private:
	void OnSphereOrbit(const Vector3& center, float radius);
	void OnEllipsoidOrbit(const Vector3& center, float a, float b, float c);

public:
	SHIP_STATE state = SHIP_STATE_EARTH_ORBIT;

	float earthOribitHeight = 10.f;
	float moonOrbitHeight = 5.f;
	float orbitRaduisZ = 100.f;
	float flySpeed = 10.f;

	TransformPtr pMoon = nullptr;
	TransformPtr pEarth = nullptr;

	float tmpDis = 10000000.f;

private:
	// 地月转移轨道与地球轨道交点
	Vector3 earthTransferPoint;
	// 月地转移轨道与月球轨道交点
	Vector3 moonTransferPoint;

	float time = 0;
	bool isGo = false;
};

typedef std::shared_ptr<Spaceship> SpaceshipPtr;