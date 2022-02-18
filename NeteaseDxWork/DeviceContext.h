#pragma once
#include <d3d11.h>

class SwapChain;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* pDeviceContext);
	~DeviceContext() = default;

public:
	bool Release();
	bool ClearRenderTargetColor(SwapChain* swapChain, float r, float g, float b, float a);

private:
	ID3D11DeviceContext* pDeviceContext = nullptr;
};

