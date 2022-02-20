#pragma once
#include <d3d11.h>

#include "GraphicsClasses.h"

class SwapChain
{
	friend class DeviceContext;
public:
	SwapChain(HWND hWnd, UINT width, UINT height, RenderSystem* rs);
	~SwapChain();

public:
	bool Present(bool vSync);

private:
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11RenderTargetView* pTargetView = nullptr;
	RenderSystem* pRenderSystem = nullptr;

private:
	void Init(HWND hWnd, UINT width, UINT height);
	void Release();
};

