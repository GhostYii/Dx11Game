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
};

