#pragma once
#include "Object.h"
#include "Matrix4x4.h"

class CameraObject : public Object
{
public:
	CameraObject(float aspect) : fov(1.57f), nearPlane(.1f), farPlane(1000.f), aspect(aspect)
	{
		pTransform->SetPosition(Vector3(0, 0, 0));
		pTransform->SetRotation(Vector3(0, 0, 0));
		pTransform->SetScale(Vector3(1, 1, 1));

		Matrix4x4 tmpView = pTransform->GetMatrix();
		tmpView.Inverse();

		view = tmpView;
		projection.SetPerpectiveFovLH(fov, aspect, nearPlane, farPlane);
	}
	virtual ~CameraObject() = default;

public:
	float fov;
	float nearPlane;
	float farPlane;

public:
	virtual void Update(float frameTime)
	{
		UpdateVP(aspect);
	}

	void UpdateVP(float aspect)
	{
		Matrix4x4 tmpView = pTransform->GetMatrix();
		tmpView.Inverse();
		view = tmpView;
		projection.SetPerpectiveFovLH(fov, aspect, nearPlane, farPlane);
	}

	Matrix4x4 GetModelMatrix()
	{
		return pTransform->GetMatrix();
	}
	Matrix4x4 GetViewMatrix()
	{
		return view;
	}
	Matrix4x4 GetProjectionMatrix()
	{
		return projection;
	}

protected:
	Matrix4x4 view;
	Matrix4x4 projection;

	float aspect = 1;
};
