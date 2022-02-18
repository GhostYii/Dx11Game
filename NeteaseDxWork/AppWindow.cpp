#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"

struct Vector3
{
	float x;
	float y;
	float z;
};

struct Vertex
{
	Vector3 position;
};

AppWindow::AppWindow()
{
}

void AppWindow::OnCreate()
{
	GraphicsEngine::GetInstance()->Init();
	pSwapChain = GraphicsEngine::GetInstance()->CreateSwapChain();
	RECT rect = this->GetClientWindowRect();
	pSwapChain->Init(this->hWnd, rect.right - rect.left, rect.bottom - rect.top);

	Vertex vertices[] =
	{
		{-.5f, -.5f, 0},
		{0, .5f, 0},
		{0.5, -.5f, 0}
	};

	pTmpVB = GraphicsEngine::GetInstance()->CreateVertexBuffer();
	UINT vertexSize = ARRAYSIZE(vertices);

	GraphicsEngine::GetInstance()->CreateShaders();

	void* shaderByteCode = nullptr;
	UINT shaderSize = 0;
	GraphicsEngine::GetInstance()->GetShaderBufferAndSize(&shaderByteCode, &shaderSize);

	pTmpVB->Load(vertices, sizeof(Vertex), vertexSize, shaderByteCode, shaderSize);
	
}

void AppWindow::OnUpdate()
{
	GraphicsEngine::GetInstance()->GetDeviceContext()->ClearRenderTargetColor(this->pSwapChain, 1, 0, 0, 1);

	RECT rect = this->GetClientWindowRect();
	GraphicsEngine::GetInstance()->GetDeviceContext()->SetViewportSize(rect.right - rect.left, rect.bottom - rect.top);
	GraphicsEngine::GetInstance()->SetShaders();

	GraphicsEngine::GetInstance()->GetDeviceContext()->SetVertexBuffer(pTmpVB);
	GraphicsEngine::GetInstance()->GetDeviceContext()->DrawTriangleList(pTmpVB->GetvertexSize(), 0);

	pSwapChain->Present(false);
}

void AppWindow::OnDestroy()
{
	Window::OnDestroy();
	pTmpVB->Release();
	pSwapChain->Release();
	GraphicsEngine::GetInstance()->Release();
}
