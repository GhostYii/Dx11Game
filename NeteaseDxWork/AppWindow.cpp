#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"

#include<Windows.h>

struct Vector3
{
	float x;
	float y;
	float z;
};

struct Vertex
{
	Vector3 position;
	Vector3 position1;
	Vector3 color;
	Vector3 color1;
};

__declspec(align(16))
struct Constant
{
	float angle;
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
		// x, y, z, c0, c1, c2,  r, g, b
		{-.5f, -.5f, 0,    -0.32f,-0.11f,0.0f,    1.f, 0, 0, 0,1,0},
		{ 0,    .5f, 0,    0.41f,0.78f,0.0f,    0, 1.f, 0, 0,0,1},
		{ .5,  -.5f, 0,    0.75f,-0.73f,0.0f,    0, 0, 1.f, 1,0,0}
	};

	pTmpVB = GraphicsEngine::GetInstance()->CreateVertexBuffer();
	UINT vertexSize = ARRAYSIZE(vertices);

	void* shaderByteCode = nullptr;
	UINT shaderSize = 0;
	GraphicsEngine::GetInstance()->CompileVertexShader(L"VertexShader.hlsl", "main", &shaderByteCode, &shaderSize);
	pTmpVS = GraphicsEngine::GetInstance()->CreateVertexShader(shaderByteCode, shaderSize);

	pTmpVB->Load(vertices, sizeof(Vertex), vertexSize, shaderByteCode, shaderSize);

	GraphicsEngine::GetInstance()->CompilePixelShader(L"PixelShader.hlsl", "main", &shaderByteCode, &shaderSize);
	pTmpPS = GraphicsEngine::GetInstance()->CreatePixelShader(shaderByteCode, shaderSize);

	GraphicsEngine::GetInstance()->ReleaseCompiledShader();

	Constant cBuffer = { 0 };

	pTmpCBuff = GraphicsEngine::GetInstance()->CreateConstantBuffer();
	pTmpCBuff->Load(&cBuffer, sizeof(Constant));
}

void AppWindow::OnUpdate()
{
	GraphicsEngine::GetInstance()->GetDeviceContext()->ClearRenderTargetColor(this->pSwapChain, 0, 0, 0, 1);

	RECT rect = this->GetClientWindowRect();
	GraphicsEngine::GetInstance()->GetDeviceContext()->SetViewportSize(rect.right - rect.left, rect.bottom - rect.top);

	unsigned long new_time = 0;
	if (m_old_time)
		new_time = ::GetTickCount() - m_old_time;
	m_delta_time = new_time / 1000.0f;
	m_old_time = ::GetTickCount();

	m_angle += 1.57f * m_delta_time;
	Constant cc;
	cc.angle = m_angle;

	pTmpCBuff->Update(GraphicsEngine::GetInstance()->GetDeviceContext(), &cc);

	GraphicsEngine::GetInstance()->GetDeviceContext()->VSSetConstantBuffer(pTmpCBuff);
	GraphicsEngine::GetInstance()->GetDeviceContext()->PSSetConstantBuffer(pTmpCBuff);

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
