#pragma once
#include "Window.h"
#include "PinelineStruct.h"
#include "GraphicsEngine.h"
#include "InputSystem.h"

#include "Math.h"

class AppWindow : public Window, public InputListener
{
public:
	AppWindow() = default;
	virtual ~AppWindow() = default;

	// Inherited via Window
	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnDestroy() override;

	// Inherited via InputLisenter
	virtual void OnKeyDown(int keycode) override;
	virtual void OnKeyUp(int keycode) override;
	virtual void OnMouseMove(const Point& mousePosition) override;
	virtual void OnMouseKeyDown(int mouseKey) override;
	virtual void OnMouseKeyUp(int mouseKey) override;

public:
	void UpdatePosition();
	//void UpdateCamera();

private:
	SwapChainPtr pSwapChain = nullptr;

private:
	VertexBufferPtr pTmpVB = nullptr;
	VertexShaderPtr pTmpVS = nullptr;
	PixelShaderPtr pTmpPS = nullptr;
	ConstantBufferPtr pTmpCBuff = nullptr;
	IndexBufferPtr pTmpIndexBuff = nullptr;
	TexturePtr pTmpTexture = nullptr;
	MeshPtr pTmpMesh = nullptr;

	unsigned long prevTime = 0;
	float prevDeltaTime = 0;
	float newDeltaTime = 0;
	float deltaTime = 0;
	
	float tmpPos = 0;
	float tmpDelta = 0;

	float tmpRotX = 0.f, tmpRotY = 0.f;
	Vector3 tmpScale = { 1,1,1 };

	float tmpForward = 0.f;
	float tmpRight = 0.f;

	Matrix4x4 worldCamMat;
};

