#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "GraphicsClasses.h"

class SwapChain
{
	friend class DeviceContext;
public:
	SwapChain(HWND hWnd, UINT width, UINT height, RenderSystem* rs);
	~SwapChain();

public:
	void Resize(unsigned int width, unsigned int height);
	void SetFullScreen(bool isFullScreen);
	bool Present(bool vSync);

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView = nullptr;
	RenderSystem* pRenderSystem = nullptr;

private:
	void Init(HWND hWnd, UINT width, UINT height);
	void Release();

	void ReloadBuffers(unsigned int width, unsigned int height);
};

