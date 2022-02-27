#pragma once
#include "ModelObject.h"

// �ɴ��ű�
class Spaceship : public ModelObject
{
public:
	enum SHIP_STATE
	{
		SHIP_STATE_EARTH_ORBIT,		// �ڵ���������
		SHIP_STATE_MOON_ORBIT,		// ������������
		SHIP_STATE_TO_EARTH,		// �µ�ת��
		SHIP_STATE_TO_MOON			// ����ת��
	};
public:
	Spaceship() = default;
	virtual ~Spaceship() = default;

public:
	virtual void Start();
	virtual void Update(float frameTime);

private:
	/// <summary>
	/// ��ȡ�����ϵĵ�����
	/// </summary>
	/// <param name="center">����</param>
	/// <param name="radius">�뾶</param>
	/// <param name="theta">�춥��</param>
	/// <param name="phi">��λ��</param>
	Vector3 GetSpherePosition(const Vector3& center, float radius, float theta, float phi);
	/// <summary>
	/// ��ȡ�����ϵĵ�����
	/// </summary>
	/// <param name="center">����ԭ��</param>
	/// <param name="a">����뾶��x����</param>
	/// <param name="b">����뾶��z����</param>
	/// <param name="c">���뾶��y����</param>
	/// <param name="theta">�춥��</param>
	/// <param name="phi">��λ��</param>
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
	// ����ת�ƹ�������������
	Vector3 earthTransferPoint;
	// �µ�ת�ƹ��������������
	Vector3 moonTransferPoint;

	float time = 0;
	bool isGo = false;
};

typedef std::shared_ptr<Spaceship> SpaceshipPtr;