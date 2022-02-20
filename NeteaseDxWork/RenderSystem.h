#pragma once
#include <d3d11.h>
#include "GraphicsClasses.h"

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem() = default;

public:
	bool Init();
	bool Release();

	SwapChain* CreateSwapChain(HWND hWnd, UINT width, UINT height);
	DeviceContext* GetDeviceContext();

	VertexBuffer* CreateVertexBuffer(void* vertices, UINT size, UINT sizes, void* shaderByteCode, UINT shaderSizeByte);
	ConstantBuffer* CreateConstantBuffer(const void* buffer, UINT bufferSize);
	IndexBuffer* CreatIndexBuffer(void* indices, UINT indicesSize);

	VertexShader* CreateVertexShader(const void* shaderByteCode, size_t byteCodeSize);
	PixelShader* CreatePixelShader(const void* shaderByteCode, size_t byteCodeSize);

public:
	bool CompileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize);
	bool CompilePixelShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize);
	void ReleaseCompiledShader();

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
	friend class ConstantBuffer;
	friend class IndexBuffer;

	friend class VertexShader;
	friend class PixelShader;

};

