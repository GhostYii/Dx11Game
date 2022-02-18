#include "SwapChain.h"
#include "GraphicsEngine.h"

bool SwapChain::Init(HWND hWnd, UINT width, UINT height)
{
    DXGI_SWAP_CHAIN_DESC scd = {};
    //ZeroMemory(scd, sizeof(scd));
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
    scd.Windowed = TRUE;

    HRESULT res = GraphicsEngine::GetInstance()->pDXGIFactory->CreateSwapChain(GraphicsEngine::GetInstance()->pDevice, &scd, &pSwapChain);

    if (FAILED(res))
        return false;

    ID3D11Texture2D* buffer = nullptr;
    res = pSwapChain->GetBuffer(0u, __uuidof(ID3D11Texture2D), (void**)&buffer);

    if (FAILED(res))
        return false;

    res= GraphicsEngine::GetInstance()->pDevice->CreateRenderTargetView(buffer, NULL, &pTargetView);
    buffer->Release();

    if (FAILED(res))
        return false;

    return true;
}

bool SwapChain::Release()
{
    pSwapChain->Release();
    delete this;
    return true;
}

bool SwapChain::Present(bool vSync)
{
    pSwapChain->Present(vSync, NULL);
    return true;
}
