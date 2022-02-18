#pragma once
#include "Window.h"
#include "SwapChain.h"

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
};

