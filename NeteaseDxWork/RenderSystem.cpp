#include "RenderSystem.h"
#include "PinelineStruct.h"

#include "d3dcompiler.h"

//#pragma comment(lib, "D3DCompiler.lib")

RenderSystem::RenderSystem() : featureLevel(D3D_FEATURE_LEVEL_11_0), pDevice(nullptr), pContext(nullptr)
{
}

bool RenderSystem::Init()
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
		return false;
	}


	pDeviceContext = new DeviceContext(pContext, this);

	pDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pDXGIDevice));
	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pDXGIAdapter));
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pDXGIFactory));

	return true;
}

bool RenderSystem::Release()
{
	if (pDXGIAdapter)
		pDXGIAdapter->Release();
	if (pDXGIDevice)
		pDXGIDevice->Release();
	if (pDXGIFactory)
		pDXGIFactory->Release();

	delete pDeviceContext;

	if (pContext)
		pContext->Release();
	if (pDevice)
		pDevice->Release();

	return true;
}

SwapChain* RenderSystem::CreateSwapChain(HWND hWnd, UINT width, UINT height)
{
	return new SwapChain(hWnd, width, height, this);
}

DeviceContext* RenderSystem::GetDeviceContext()
{
	return this->pDeviceContext;
}

VertexBuffer* RenderSystem::CreateVertexBuffer(void* vertices, UINT size, UINT sizes, void* shaderByteCode, UINT shaderSizeByte)
{
	return new VertexBuffer(vertices, size, sizes, shaderByteCode, shaderSizeByte, this);
}

ConstantBuffer* RenderSystem::CreateConstantBuffer(const void* buffer, UINT bufferSize)
{
	return new ConstantBuffer(buffer, bufferSize, this);
}

IndexBuffer* RenderSystem::CreatIndexBuffer(void* indices, UINT indicesSize)
{
	return new IndexBuffer(indices, indicesSize, this);
}

VertexShader* RenderSystem::CreateVertexShader(const void* shaderByteCode, size_t byteCodeSize)
{
	VertexShader* vs = new VertexShader(shaderByteCode, byteCodeSize, this);
	return vs;
}

PixelShader* RenderSystem::CreatePixelShader(const void* shaderByteCode, size_t byteCodeSize)
{
	PixelShader* ps = new PixelShader(shaderByteCode, byteCodeSize, this);
	return ps;
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
