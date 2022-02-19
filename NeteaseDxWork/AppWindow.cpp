#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "Math.h"
#include "InputSystem.h"

#include<Windows.h>


struct Vertex
{
	Vector3 position;
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
	InputSystem::GetInstance()->AddListener(this);

	GraphicsEngine::GetInstance()->Init();
	pSwapChain = GraphicsEngine::GetInstance()->CreateSwapChain();
	RECT rect = this->GetClientWindowRect();
	pSwapChain->Init(this->hWnd, rect.right - rect.left, rect.bottom - rect.top);

	Vertex vertices[] =
	{
		//X - Y - Z
		//FRONT FACE
		{Vector3(-0.5f,-0.5f,-0.5f), Vector3(1,0,0), Vector3(0.2f,0,0) },
		{Vector3(-0.5f,0.5f,-0.5f),  Vector3(1,1,0), Vector3(0.2f,0.2f,0) },
		{ Vector3(0.5f,0.5f,-0.5f),  Vector3(1,1,0), Vector3(0.2f,0.2f,0) },
		{ Vector3(0.5f,-0.5f,-0.5f), Vector3(1,0,0), Vector3(0.2f,0,0) },

		//BACK FACE
		{ Vector3(0.5f,-0.5f,0.5f),  Vector3(0,1,0), Vector3(0,0.2f,0) },
		{ Vector3(0.5f,0.5f,0.5f),   Vector3(0,1,1), Vector3(0,0.2f,0.2f) },
		{ Vector3(-0.5f,0.5f,0.5f),  Vector3(0,1,1), Vector3(0,0.2f,0.2f) },
		{ Vector3(-0.5f,-0.5f,0.5f), Vector3(0,1,0), Vector3(0,0.2f,0) }
	};

	pTmpVB = GraphicsEngine::GetInstance()->CreateVertexBuffer();
	UINT vertexSize = ARRAYSIZE(vertices);

	unsigned int indexList[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		1,6,5,
		5,2,1,
		//BOTTOM SIDE
		7,0,3,
		3,4,7,
		//RIGHT SIDE
		3,2,5,
		5,4,3,
		//LEFT SIDE
		7,6,1,
		1,0,7
	};

	pTmpIndexBuff = GraphicsEngine::GetInstance()->CreatIndexBuffer();
	UINT indexListSize = ARRAYSIZE(indexList);

	pTmpIndexBuff->Load(indexList, indexListSize);

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
	InputSystem::GetInstance()->Update();

	GraphicsEngine::GetInstance()->GetDeviceContext()->ClearRenderTargetColor(this->pSwapChain, 0, 0, 0, 1);

	RECT rect = this->GetClientWindowRect();
	GraphicsEngine::GetInstance()->GetDeviceContext()->SetViewportSize(rect.right - rect.left, rect.bottom - rect.top);

	UpdatePosition();

	GraphicsEngine::GetInstance()->GetDeviceContext()->VSSetConstantBuffer(pTmpCBuff);
	GraphicsEngine::GetInstance()->GetDeviceContext()->PSSetConstantBuffer(pTmpCBuff);

	GraphicsEngine::GetInstance()->GetDeviceContext()->SetVertexShader(pTmpVS);
	GraphicsEngine::GetInstance()->GetDeviceContext()->SetPixelShader(pTmpPS);

	GraphicsEngine::GetInstance()->GetDeviceContext()->SetVertexBuffer(pTmpVB);
	GraphicsEngine::GetInstance()->GetDeviceContext()->SetIndexBuffer(pTmpIndexBuff);
	GraphicsEngine::GetInstance()->GetDeviceContext()->DrawIndexedTriangleList(pTmpIndexBuff->GetIndexListSize(), 0, 0);

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

	pTmpIndexBuff->Release();
	pTmpVS->Release();
	pTmpPS->Release();

	GraphicsEngine::GetInstance()->Release();
}

void AppWindow::UpdatePosition()
{
	// Scale - Rotation - Translate

	Constant c = {};
	c.time = GetTickCount();

	tmpPos += deltaTime / 10.f;
	if (tmpPos > 1.f)
		tmpPos = 0;

	Matrix4x4 tmpMat;

	tmpDelta += deltaTime / .5f;	

	//c.world.SetScale(Vector3::Lerp(Vector3(0, 0, 0), Vector3(1, 1, 0), (sin(tmpDelta) + 1.f) / 2.f));
	//tmpMat.SetTranslate(Vector3::Lerp(Vector3(-1.5f, -1.5f, 0), Vector3(1.5f, 1.5f, 0), tmpPos));

	//c.world *= tmpMat;

	c.world.SetScale(Vector3(1, 1, 1));

	tmpMat.SetIdentity();
	tmpMat.SetRotationZ(0);
	c.world *= tmpMat;

	tmpMat.SetIdentity();
	tmpMat.SetRotationY(tmpRotY);
	c.world *= tmpMat;

	tmpMat.SetIdentity();
	tmpMat.SetRotationX(tmpRotX);
	c.world *= tmpMat;

	c.view.SetIdentity();
	c.projection.SetOrthoLH
	(
		(GetClientWindowRect().right - GetClientWindowRect().left) / 400.f,
		(GetClientWindowRect().bottom - GetClientWindowRect().top) / 400.f,
		-4.f, 4.f
	);

	pTmpCBuff->Update(GraphicsEngine::GetInstance()->GetDeviceContext(), &c);
}

void AppWindow::OnKeyDown(int keycode)
{
	if (keycode == 'W')
	{
		tmpRotX += deltaTime;
	}
	else if (keycode == 'S')
	{
		tmpRotX -= deltaTime;
	}
	else if (keycode == 'A')
	{
		tmpRotY += deltaTime;
	}
	else if (keycode == 'D')
	{
		tmpRotY -= deltaTime;
	}

}

void AppWindow::OnKeyUp(int keycode)
{
}
