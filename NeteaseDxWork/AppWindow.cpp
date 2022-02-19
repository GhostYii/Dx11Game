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
	Vector3 color;
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
		// x, y, z, r, g, b
		{-.5f, -.5f, 0, 1.f, 0,   0,},
		{ 0,    .5f, 0, 0,   1.f, 0},
		{ .5,  -.5f, 0, 0,   0,   1.f}
	};

	pTmpVB = GraphicsEngine::GetInstance()->CreateVertexBuffer();
	UINT vertexSize = ARRAYSIZE(vertices);

	//GraphicsEngine::GetInstance()->CreateShaders();

	void* shaderByteCode = nullptr;
	UINT shaderSize = 0;
	GraphicsEngine::GetInstance()->CompileVertexShader(L"VertexShader.hlsl", "main", &shaderByteCode, &shaderSize);
	pTmpVS = GraphicsEngine::GetInstance()->CreateVertexShader(shaderByteCode, shaderSize);

	pTmpVB->Load(vertices, sizeof(Vertex), vertexSize, shaderByteCode, shaderSize);

	GraphicsEngine::GetInstance()->CompilePixelShader(L"PixelShader.hlsl", "main", &shaderByteCode, &shaderSize);
	pTmpPS = GraphicsEngine::GetInstance()->CreatePixelShader(shaderByteCode, shaderSize);

	GraphicsEngine::GetInstance()->ReleaseCompiledShader();
}

void AppWindow::OnUpdate()
{
	GraphicsEngine::GetInstance()->GetDeviceContext()->ClearRenderTargetColor(this->pSwapChain, 0, 0, 0, 1);

	RECT rect = this->GetClientWindowRect();
	GraphicsEngine::GetInstance()->GetDeviceContext()->SetViewportSize(rect.right - rect.left, rect.bottom - rect.top);

	GraphicsEngine::GetInstance()->GetDeviceContext()->SetVertexShader(pTmpVS);
	GraphicsEngine::GetInstance()->GetDeviceContext()->SetPixelShader(pTmpPS);

	GraphicsEngine::GetInstance()->GetDeviceContext()->SetVertexBuffer(pTmpVB);
	GraphicsEngine::GetInstance()->GetDeviceContext()->DrawTriangleList(pTmpVB->GetvertexSize(), 0);

	pSwapChain->Present(false);
}

void AppWindow::OnDestroy()
{
	Window::OnDestroy();
	pTmpVB->Release();
	pSwapChain->Release();

	pTmpVS->Release();
	pTmpPS->Release();

	GraphicsEngine::GetInstance()->Release();
}
