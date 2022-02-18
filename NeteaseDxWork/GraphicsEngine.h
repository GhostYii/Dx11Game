#pragma once
#include <d3d11.h>

class SwapChain;
class DeviceContext;

class GraphicsEngine
{
	friend class SwapChain;
public:
	GraphicsEngine();
	~GraphicsEngine() = default;

public:
	bool Init();
	bool Release();

	SwapChain* CreateSwapChain();
	DeviceContext* GetDeviceContext();

	static GraphicsEngine* GetInstance();

private:
	DeviceContext* pDeviceContext = nullptr;

private:
	ID3D11Device* pDevice = nullptr;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11DeviceContext* pContext = nullptr;
	
	IDXGIDevice* pDXGIDevice = nullptr;
	IDXGIAdapter* pDXGIAdapter = nullptr;
	IDXGIFactory* pDXGIFactory = nullptr;
	

};

