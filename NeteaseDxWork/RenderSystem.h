#pragma once
#include <d3d11.h>
#include "GraphicsClasses.h"

class RenderSystem
{
	friend class SwapChain;

	friend class VertexBuffer;
	friend class ConstantBuffer;
	friend class IndexBuffer;

	friend class VertexShader;
	friend class PixelShader;

	friend class Texture;
public:
	RenderSystem();
	~RenderSystem();

public:
	SwapChainPtr CreateSwapChain(HWND hWnd, UINT width, UINT height);
	DeviceContextPtr GetDeviceContext();

	VertexBufferPtr CreateVertexBuffer(void* vertices, UINT size, UINT sizes, void* shaderByteCode, UINT shaderSizeByte);
	ConstantBufferPtr CreateConstantBuffer(const void* buffer, UINT bufferSize);
	IndexBufferPtr CreateIndexBuffer(void* indices, UINT indicesSize);

	VertexShaderPtr CreateVertexShader(const void* shaderByteCode, size_t byteCodeSize);
	PixelShaderPtr CreatePixelShader(const void* shaderByteCode, size_t byteCodeSize);

	void SetRasterizerState(D3D11_CULL_MODE mode);

public:
	bool CompileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize);
	bool CompilePixelShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize);
	void ReleaseCompiledShader();

private:
	DeviceContextPtr pDeviceContext = nullptr;
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
	ID3D11RasterizerState* pCullFrontState = nullptr;
	ID3D11RasterizerState* pCullBackState = nullptr;

private:
	void Init();
	void Release();

	void InitRasterizerState();
};

