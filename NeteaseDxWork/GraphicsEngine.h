#pragma once
#include <d3d11.h>

class SwapChain;
class DeviceContext;
class VertexBuffer;

class GraphicsEngine
{
	

public:
	GraphicsEngine();
	~GraphicsEngine() = default;

public:
	bool Init();
	bool Release();

	SwapChain* CreateSwapChain();
	DeviceContext* GetDeviceContext();
	VertexBuffer* CreateVertexBuffer();

	static GraphicsEngine* GetInstance();

public:
	bool CreateShaders();
	bool SetShaders();
	void GetShaderBufferAndSize(void** bytecode, UINT* size);

private:
	DeviceContext* pDeviceContext = nullptr;

private:
	ID3D11Device* pDevice = nullptr;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11DeviceContext* pContext = nullptr;
	
	IDXGIDevice* pDXGIDevice = nullptr;
	IDXGIAdapter* pDXGIAdapter = nullptr;
	IDXGIFactory* pDXGIFactory = nullptr;

private:
	ID3DBlob* pVSBlob = nullptr;
	ID3DBlob* pPSBlob = nullptr;
	ID3D11VertexShader* pVs = nullptr;
	ID3D11PixelShader* pPs = nullptr;
	
private:
	friend class SwapChain;
	friend class VertexBuffer;
};

