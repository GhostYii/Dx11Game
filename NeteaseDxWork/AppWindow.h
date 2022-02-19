#pragma once
#include "Window.h"
#include "PinelineStruct.h"
#include "InputListener.h"

class AppWindow : public Window, public InputLisenter
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

public:
	void UpdatePosition();

private:
	SwapChain* pSwapChain = nullptr;

private:
	VertexBuffer* pTmpVB = nullptr;
	VertexShader* pTmpVS = nullptr;
	PixelShader* pTmpPS = nullptr;
	ConstantBuffer* pTmpCBuff = nullptr;
	IndexBuffer* pTmpIndexBuff = nullptr;

	unsigned long prevTime = 0;
	float prevDeltaTime = 0;
	float newDeltaTime = 0;
	float deltaTime = 0;
	
	float tmpPos = 0;
	float tmpDelta = 0;

	float tmpRotX = 0.f, tmpRotY = 0.f;
};

