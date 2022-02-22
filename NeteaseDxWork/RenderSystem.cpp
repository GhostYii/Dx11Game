#include "RenderSystem.h"
#include "PinelineStruct.h"

#include "d3dcompiler.h"
#include <exception>

//#pragma comment(lib, "D3DCompiler.lib")

RenderSystem::RenderSystem() : featureLevel(D3D_FEATURE_LEVEL_11_0), pDevice(nullptr), pContext(nullptr)
{
	Init();
}

RenderSystem::~RenderSystem()
{
	Release();
}

void RenderSystem::Init()
{
	D3D_DRIVER_TYPE deviceType[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};

	HRESULT res = 0;
	//ID3D11DeviceContext* pDx11DeviceContext = nullptr;
	for (auto& driver : deviceType)
	{
		res = D3D11CreateDevice(NULL, driver, NULL, NULL, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &pDevice, &featureLevel, &pContext);

		if (SUCCEEDED(res))
			break;
	}

	if (FAILED(res))
	{
		throw std::exception("CreateDevice failed!");
	}

	pDeviceContext = std::make_shared<DeviceContext>(pContext, this);

	pDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pDXGIDevice));
	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pDXGIAdapter));
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pDXGIFactory));

	InitRasterizerState();
}

void RenderSystem::Release()
{
	if (pDXGIAdapter)
		pDXGIAdapter->Release();
	if (pDXGIDevice)
		pDXGIDevice->Release();
	if (pDXGIFactory)
		pDXGIFactory->Release();

	if (pContext)
		pContext->Release();
	if (pDevice)
		pDevice->Release();

	if (pCullFrontState)
		pCullFrontState->Release();
	if (pCullBackState)
		pCullBackState->Release();
}

void RenderSystem::InitRasterizerState()
{
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_FRONT;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_SOLID;
	pDevice->CreateRasterizerState(&desc, &pCullFrontState);

	desc.CullMode = D3D11_CULL_BACK;
	pDevice->CreateRasterizerState(&desc, &pCullBackState);
}

SwapChainPtr RenderSystem::CreateSwapChain(HWND hWnd, UINT width, UINT height)
{
	SwapChainPtr ptr = nullptr;
	try
	{
		ptr = std::make_shared<SwapChain>(hWnd, width, height, this);
	}
	catch (...) {}

	return ptr;
}

DeviceContextPtr RenderSystem::GetDeviceContext()
{
	return this->pDeviceContext;
}

VertexBufferPtr RenderSystem::CreateVertexBuffer(void* vertices, UINT size, UINT sizes, void* shaderByteCode, UINT shaderSizeByte)
{
	VertexBufferPtr ptr = nullptr;
	try
	{
		ptr = std::make_shared<VertexBuffer>(vertices, size, sizes, shaderByteCode, shaderSizeByte, this);
	}
	catch (...) {}

	return ptr;
}

ConstantBufferPtr RenderSystem::CreateConstantBuffer(const void* buffer, UINT bufferSize)
{
	ConstantBufferPtr ptr = nullptr;
	try
	{
		ptr = std::make_shared<ConstantBuffer>(buffer, bufferSize, this);
	}
	catch (...) {}

	return ptr;
}

IndexBufferPtr RenderSystem::CreateIndexBuffer(void* indices, UINT indicesSize)
{
	IndexBufferPtr ptr = nullptr;
	try
	{
		ptr = std::make_shared<IndexBuffer>(indices, indicesSize, this);
	}
	catch (...) {}

	return ptr;
}

VertexShaderPtr RenderSystem::CreateVertexShader(const void* shaderByteCode, size_t byteCodeSize)
{
	VertexShaderPtr ptr = nullptr;
	try
	{
		ptr = std::make_shared<VertexShader>(shaderByteCode, byteCodeSize, this);
	}
	catch (...) {}

	return ptr;
}

PixelShaderPtr RenderSystem::CreatePixelShader(const void* shaderByteCode, size_t byteCodeSize)
{
	PixelShaderPtr ptr = nullptr;
	try
	{
		ptr = std::make_shared<PixelShader>(shaderByteCode, byteCodeSize, this);
	}
	catch (...) {}

	return ptr;
}

void RenderSystem::SetRasterizerState(D3D11_CULL_MODE mode)
{
	switch (mode)
	{
	case D3D11_CULL_NONE:
		break;
	case D3D11_CULL_FRONT:
		pContext->RSSetState(pCullFrontState);
		break;
	case D3D11_CULL_BACK:
		pContext->RSSetState(pCullBackState);
		break;
	default:
		break;
	}
}

bool RenderSystem::CompileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize)
{
	ID3DBlob* errorBlob = nullptr;
	HRESULT res = D3DCompileFromFile(fileName, nullptr, nullptr, entryPointName, "vs_5_0", 0, 0, &pVSBlob, &errorBlob);

	if (!SUCCEEDED(res))
	{
		if (errorBlob)
			errorBlob->Release();
		return false;
	}

	*shaderByteCode = pVSBlob->GetBufferPointer();
	*byteCodeSize = pVSBlob->GetBufferSize();

	return true;
}

bool RenderSystem::CompilePixelShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize)
{
	ID3DBlob* errorBlob = nullptr;
	HRESULT res = D3DCompileFromFile(fileName, nullptr, nullptr, entryPointName, "ps_5_0", 0, 0, &pPSBlob, &errorBlob);

	if (!SUCCEEDED(res))
	{
		if (errorBlob)
			errorBlob->Release();
		return false;
	}

	*shaderByteCode = pPSBlob->GetBufferPointer();
	*byteCodeSize = pPSBlob->GetBufferSize();

	return true;
}

void RenderSystem::ReleaseCompiledShader()
{
	if (pVSBlob)
		pVSBlob->Release();

	if (pPSBlob)
		pPSBlob->Release();
}
