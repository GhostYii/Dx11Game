#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "Math.h"

#include<Windows.h>


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
	Matrix4x4 world;
	Matrix4x4 view;
	Matrix4x4 projection;
	UINT time;
};

void AppWindow::OnCreate()
{
	GraphicsEngine::GetInstance()->Init();
	pSwapChain = GraphicsEngine::GetInstance()->CreateSwapChain();
	RECT rect = this->GetClientWindowRect();
	pSwapChain->Init(this->hWnd, rect.right - rect.left, rect.bottom - rect.top);

	Vertex vertices[] =
	{
		// x, y, z, c0, c1, c2,  r, g, b
		{ Vector3(-.5f, -.5f, 0), Vector3(-0.32f, -0.11f, 0.0f), Vector3(1.f, 0, 0), Vector3(0, 1, 0) },
		{ Vector3(0, .5f, 0), Vector3(0.41f, 0.78f, 0.0f), Vector3(0, 1.f, 0), Vector3(0, 0, 1) },
		{ Vector3(.5,  -.5f, 0), Vector3(0.75f, -0.73f, 0.0f), Vector3(0, 0, 1.f), Vector3(1, 0, 0) }
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

	UpdatePosition();

	GraphicsEngine::GetInstance()->GetDeviceContext()->VSSetConstantBuffer(pTmpCBuff);
	GraphicsEngine::GetInstance()->GetDeviceContext()->PSSetConstantBuffer(pTmpCBuff);

	GraphicsEngine::GetInstance()->GetDeviceContext()->SetVertexShader(pTmpVS);
	GraphicsEngine::GetInstance()->GetDeviceContext()->SetPixelShader(pTmpPS);

	GraphicsEngine::GetInstance()->GetDeviceContext()->SetVertexBuffer(pTmpVB);
	GraphicsEngine::GetInstance()->GetDeviceContext()->DrawTriangleList(pTmpVB->GetvertexSize(), 0);

	pSwapChain->Present(false);

	prevDeltaTime = newDeltaTime;
	newDeltaTime = GetTickCount64();
	deltaTime = prevDeltaTime ? (newDeltaTime - prevDeltaTime) / 1000.f : 0;
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

void AppWindow::UpdatePosition()
{
	Constant c = {};
	c.time = GetTickCount();

	deltaPos += deltaTime / .5f;

	c.world.SetScale(Vector3::Lerp(Vector3(0, 0, 0), Vector3(1, 1, 0), (sin(deltaPos)+1.f)/2.f));
	c.view.SetIdentity();
	c.projection.SetOrthoLH
	(
		(GetClientWindowRect().right - GetClientWindowRect().left) / 400.f,
		(GetClientWindowRect().bottom - GetClientWindowRect().top) / 400.f,
		-4.f, 4.f
	);

	pTmpCBuff->Update(GraphicsEngine::GetInstance()->GetDeviceContext(), &c);
}
