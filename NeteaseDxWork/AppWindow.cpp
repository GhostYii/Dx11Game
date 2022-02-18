#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"

AppWindow::AppWindow()
{
}

void AppWindow::OnCreate()
{
	GraphicsEngine::GetInstance()->Init();
	pSwapChain = GraphicsEngine::GetInstance()->CreateSwapChain();
	RECT rect = this->GetClientWindowRect();
	pSwapChain->Init(this->hWnd, rect.right - rect.left, rect.bottom - rect.top);
}

void AppWindow::OnUpdate()
{
	GraphicsEngine::GetInstance()->GetDeviceContext()->ClearRenderTargetColor(this->pSwapChain, 1, 0, 0, 1);
	pSwapChain->Present(false);
}

void AppWindow::OnDestroy()
{
	Window::OnDestroy();
	pSwapChain->Release();
	GraphicsEngine::GetInstance()->Release();
}
