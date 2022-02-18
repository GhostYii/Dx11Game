#include "DeviceContext.h"
#include "SwapChain.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* pDeviceContext) :pDeviceContext(pDeviceContext)
{
}

bool DeviceContext::Release()
{
	pDeviceContext->Release();
	delete this;
	return true;
}

bool DeviceContext::ClearRenderTargetColor(SwapChain* swapChain, float r, float g, float b, float a)
{
	const float color[] = { r,g,b,a };
	pDeviceContext->ClearRenderTargetView(swapChain->pTargetView, color);
	return true;
}
