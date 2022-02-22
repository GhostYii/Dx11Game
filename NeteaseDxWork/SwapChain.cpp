#include "SwapChain.h"
#include "RenderSystem.h"

#include <exception>

void SwapChain::Init(HWND hWnd, UINT width, UINT height)
{
	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferCount = 1;
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.Windowed = TRUE;

	HRESULT res = pRenderSystem->pDXGIFactory->CreateSwapChain(pRenderSystem->pDevice, &scd, pSwapChain.GetAddressOf());

	if (FAILED(res))
		throw std::exception("Create SwapChain failed!");

	ReloadBuffers(width, height);
}

void SwapChain::Release()
{
	pSwapChain->Release();
}

void SwapChain::ReloadBuffers(unsigned int width, unsigned int height)
{
	ID3D11Texture2D* buffer = nullptr;
	HRESULT res = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(res))
		throw std::exception("SwapChain get buffer failed!");

	res = pRenderSystem->pDevice->CreateRenderTargetView(buffer, nullptr, pRenderTargetView.GetAddressOf());
	buffer->Release();

	if (FAILED(res))
		throw std::exception("SwapChain CreateDepthStencilView failed!");

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.MiscFlags = 0;
	desc.ArraySize = 1;
	desc.CPUAccessFlags = 0;

	res = pRenderSystem->pDevice->CreateTexture2D(&desc, nullptr, &buffer);

	if (FAILED(res))
		throw std::exception("SwapChain CreateTexture2D failed!");

	res = pRenderSystem->pDevice->CreateDepthStencilView(buffer, nullptr, pDepthStencilView.GetAddressOf());
	buffer->Release();

	if (FAILED(res))
		throw std::exception("SwapChain CreateDepthStencilView failed!");
}

SwapChain::SwapChain(HWND hWnd, UINT width, UINT height, RenderSystem* rs) : pRenderSystem(rs)
{
	Init(hWnd, width, height);
}

SwapChain::~SwapChain()
{
	Release();
}

void SwapChain::Resize(unsigned int width, unsigned int height)
{
	if (pRenderTargetView)
		pRenderTargetView.Reset();
	if (pDepthStencilView)
		pDepthStencilView.Reset();

	pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ReloadBuffers(width, height);
}

void SwapChain::SetFullScreen(bool isFullScreen)
{
	Resize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	pSwapChain->SetFullscreenState(isFullScreen, nullptr);	
}

bool SwapChain::Present(bool vSync)
{
	pSwapChain->Present(vSync, NULL);
	return true;
}
