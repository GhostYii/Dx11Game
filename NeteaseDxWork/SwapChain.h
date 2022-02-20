#pragma once
#include <d3d11.h>

#include "GraphicsClasses.h"

class SwapChain
{
	friend class DeviceContext;
public:
	SwapChain(RenderSystem* rs) : pRenderSystem(rs) {}
	~SwapChain() = default;

public:
	bool Init(HWND hWnd, UINT width, UINT height);
	bool Release();

	bool Present(bool vSync);

private:
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11RenderTargetView* pTargetView = nullptr;
	RenderSystem* pRenderSystem = nullptr;
};

