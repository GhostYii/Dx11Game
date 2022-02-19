#include "GraphicsEngine.h"
#include "PinelineStruct.h"

#include "d3dcompiler.h"

//#pragma comment(lib, "D3DCompiler.lib")

GraphicsEngine::GraphicsEngine() : featureLevel(D3D_FEATURE_LEVEL_11_0), pDevice(nullptr), pContext(nullptr)
{
}

bool GraphicsEngine::Init()
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


	pDeviceContext = new DeviceContext(pContext);

	pDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pDXGIDevice));
	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pDXGIAdapter));
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pDXGIFactory));

	return true;
}

bool GraphicsEngine::Release()
{
	if (pDXGIAdapter)
		pDXGIAdapter->Release();
	if (pDXGIDevice)
		pDXGIDevice->Release();
	if (pDXGIFactory)
		pDXGIFactory->Release();

	if (pDeviceContext)
		pDeviceContext->Release();
	if (pContext)
		pContext->Release();
	if (pDevice)
		pDevice->Release();

	return true;
}

SwapChain* GraphicsEngine::CreateSwapChain()
{
	return new SwapChain();
}

DeviceContext* GraphicsEngine::GetDeviceContext()
{
	return this->pDeviceContext;
}

VertexBuffer* GraphicsEngine::CreateVertexBuffer()
{
	return new VertexBuffer();
}

ConstantBuffer* GraphicsEngine::CreateConstantBuffer()
{
	return new ConstantBuffer();
}

IndexBuffer* GraphicsEngine::CreatIndexBuffer()
{
	return new IndexBuffer();
}

VertexShader* GraphicsEngine::CreateVertexShader(const void* shaderByteCode, size_t byteCodeSize)
{
	VertexShader* vs = new VertexShader();

	if (!vs->Init(shaderByteCode, byteCodeSize))
	{
		vs->Release();
		return nullptr;
	}
	return vs;
}

PixelShader* GraphicsEngine::CreatePixelShader(const void* shaderByteCode, size_t byteCodeSize)
{
	PixelShader* ps = new PixelShader();

	if (!ps->Init(shaderByteCode, byteCodeSize))
	{
		ps->Release();
		return nullptr;
	}
	return ps;
}

GraphicsEngine* GraphicsEngine::GetInstance()
{
	static GraphicsEngine engine;

	return &engine;
}

bool GraphicsEngine::CompileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize)
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

bool GraphicsEngine::CompilePixelShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize)
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

void GraphicsEngine::ReleaseCompiledShader()
{
	if (pVSBlob)
		pVSBlob->Release();

	if (pPSBlob)
		pPSBlob->Release();
}
