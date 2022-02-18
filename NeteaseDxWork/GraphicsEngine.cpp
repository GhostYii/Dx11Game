#include "GraphicsEngine.h"

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
	
	pDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pDXGIDevice));
	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pDXGIAdapter));
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pDXGIFactory));

	return true;
}

bool GraphicsEngine::Release()
{
	pDXGIAdapter->Release();	
	pDXGIDevice->Release();
	pDXGIFactory->Release();

	pContext->Release();
	pDevice->Release();
	return false;
}

SwapChain* GraphicsEngine::CreateSwapChain()
{
	return new SwapChain();
}

GraphicsEngine* GraphicsEngine::GetInstance()
{
	static GraphicsEngine engine;

	return &engine;
}
