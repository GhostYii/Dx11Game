#include "AppWindow.h"
#include "GraphicsEngine.h"

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
}

void AppWindow::OnDestroy()
{
	Window::OnDestroy();
	GraphicsEngine::GetInstance()->Release();
}
