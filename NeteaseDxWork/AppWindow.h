#pragma once
#include "Window.h"
#include "PinelineStruct.h"

class AppWindow : public Window
{
public:
	AppWindow();
	virtual ~AppWindow() = default;

	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnDestroy() override;

private:
	SwapChain* pSwapChain = nullptr;

private:
	VertexBuffer* pTmpVB = nullptr;
	VertexShader* pTmpVS = nullptr;
	PixelShader* pTmpPS = nullptr;
	ConstantBuffer* pTmpCBuff = nullptr;

	unsigned long m_old_time = 0;
	float m_delta_time = 0;
	float m_angle = 0;
};

