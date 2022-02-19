#pragma once
#include "Window.h"
#include "PinelineStruct.h"

class AppWindow : public Window
{
public:
	AppWindow() = default;
	virtual ~AppWindow() = default;

	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnDestroy() override;

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
};

