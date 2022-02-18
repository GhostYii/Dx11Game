#pragma once
#include <d3d11.h>

class SwapChain
{
public:
	SwapChain() = default;
	~SwapChain() = default;

public:
	bool Init(HWND hWnd, UINT width, UINT height);
	bool Release();

private:
	IDXGISwapChain* pSwapChain = nullptr;
};

