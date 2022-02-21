#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "Mesh.h"
#include "InputSystem.h"

#include<Windows.h>


struct Vertex
{
	Vector3 position;
	Vector2 texcoord;
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

	//InputSystem::GetInstance()->SetCursorVisiable(false);
	pTmpTexture = GraphicsEngine::GetInstance()->GetTextureManger()->CreateTextureFromFile(L"Assets\\Textures\\r_earth.jpg");
	pTmpMesh = GraphicsEngine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\sphere.obj");

	RECT rect = this->GetClientWindowRect();
	pSwapChain = GraphicsEngine::GetInstance()->GetRenderSystem()->CreateSwapChain(this->hWnd, rect.right - rect.left, rect.bottom - rect.top);

	worldCamMat.SetTranslate(Vector3(0, 0, -10.f));

#pragma region Cube
	//Vector3 vertics[] =
	//{
	//	//		X - Y - Z
	//	//FRONT FACE
	//	{ -0.5f,-0.5f,-0.5f },
	//	{ -0.5f,0.5f,-0.5f },
	//	{ 0.5f,0.5f,-0.5f },
	//	{ 0.5f,-0.5f,-0.5f },

	//	//BACK FACE
	//	{ 0.5f,-0.5f,0.5f },
	//	{ 0.5f,0.5f,0.5f },
	//	{ -0.5f,0.5f,0.5f },
	//	{ -0.5f,-0.5f,0.5f }
	//};

	//Vector2 texcoords[] =
	//{
	//	{ 0, 0 },
	//	{ 0, 1.f },
	//	{ 1.f, 0 },
	//	{ 1.f, 1.f }
	//};

	//Vertex vertices[] =
	//{
	//	{ vertics[0], texcoords[1] },
	//	{ vertics[1], texcoords[0] },
	//	{ vertics[2], texcoords[2] },
	//	{ vertics[3], texcoords[3] },

	//	{ vertics[4], texcoords[1] },
	//	{ vertics[5], texcoords[0] },
	//	{ vertics[6], texcoords[2] },
	//	{ vertics[7], texcoords[3] },

	//	{ vertics[1], texcoords[1] },
	//	{ vertics[6], texcoords[0] },
	//	{ vertics[5], texcoords[2] },
	//	{ vertics[2], texcoords[3] },

	//	{ vertics[7], texcoords[1] },
	//	{ vertics[0], texcoords[0] },
	//	{ vertics[3], texcoords[2] },
	//	{ vertics[4], texcoords[3] },

	//	{ vertics[3], texcoords[1] },
	//	{ vertics[2], texcoords[0] },
	//	{ vertics[5], texcoords[2] },
	//	{ vertics[4], texcoords[3] },

	//	{ vertics[7], texcoords[1] },
	//	{ vertics[6], texcoords[0] },
	//	{ vertics[1], texcoords[2] },
	//	{ vertics[0], texcoords[3] },
	//};


	//UINT vertexSize = ARRAYSIZE(vertices);

	//unsigned int indexList[] =
	//{
	//	//FRONT SIDE
	//	0,1,2,  //FIRST TRIANGLE
	//	2,3,0,  //SECOND TRIANGLE
	//	//BACK SIDE
	//	4,5,6,
	//	6,7,4,
	//	//TOP SIDE
	//	8,9,10,
	//	10,11,8,
	//	//BOTTOM SIDE
	//	12,13,14,
	//	14,15,12,
	//	//RIGHT SIDE
	//	16,17,18,
	//	18,19,16,
	//	//LEFT SIDE
	//	20,21,22,
	//	22,23,20
	//};

	//UINT indexListSize = ARRAYSIZE(indexList);
	//pTmpIndexBuff = GraphicsEngine::GetInstance()->GetRenderSystem()->CreateIndexBuffer(indexList, indexListSize);
#pragma endregion

	void* shaderByteCode = nullptr;
	UINT shaderSize = 0;
	GraphicsEngine::GetInstance()->GetRenderSystem()->CompileVertexShader(L"VertexShader.hlsl", "main", &shaderByteCode, &shaderSize);
	pTmpVS = GraphicsEngine::GetInstance()->GetRenderSystem()->CreateVertexShader(shaderByteCode, shaderSize);

	//pTmpVB = GraphicsEngine::GetInstance()->GetRenderSystem()->CreateVertexBuffer(vertices, sizeof(Vertex), vertexSize, shaderByteCode, shaderSize);

	bool isBuild = GraphicsEngine::GetInstance()->GetRenderSystem()->CompilePixelShader(L"PixelShader.hlsl", "main", &shaderByteCode, &shaderSize);
	pTmpPS = GraphicsEngine::GetInstance()->GetRenderSystem()->CreatePixelShader(shaderByteCode, shaderSize);

	GraphicsEngine::GetInstance()->GetRenderSystem()->ReleaseCompiledShader();

	Constant cBuffer = { 0 };

	pTmpCBuff = GraphicsEngine::GetInstance()->GetRenderSystem()->CreateConstantBuffer(&cBuffer, sizeof(Constant));
}

void AppWindow::OnUpdate()
{
	InputSystem::GetInstance()->Update();

	GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext()->ClearRenderTargetColor(this->pSwapChain, 0, 0, 0, 1);

	RECT rect = this->GetClientWindowRect();
	GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext()->SetViewportSize(rect.right - rect.left, rect.bottom - rect.top);

	UpdatePosition();

	GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext()->VSSetConstantBuffer(pTmpCBuff);
	GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext()->PSSetConstantBuffer(pTmpCBuff);

	GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext()->SetVertexShader(pTmpVS);
	GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext()->SetPixelShader(pTmpPS);

	GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext()->VSSetTexture(pTmpTexture);
	GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext()->PSSetTexture(pTmpTexture);

	//GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext()->SetVertexBuffer(pTmpVB);
	//GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext()->SetIndexBuffer(pTmpIndexBuff);
	//GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext()->DrawIndexedTriangleList(pTmpIndexBuff->GetIndexListSize(), 0, 0);
	GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext()->SetVertexBuffer(pTmpMesh->GetVertexBuffer());
	GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext()->SetIndexBuffer(pTmpMesh->GetIndexBuffer());
	GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext()->DrawIndexedTriangleList(pTmpMesh->GetIndexBuffer()->GetIndexListSize(), 0, 0);

	pSwapChain->Present(false);

	prevDeltaTime = newDeltaTime;
	newDeltaTime = GetTickCount();
	deltaTime = prevDeltaTime ? (newDeltaTime - prevDeltaTime) / 1000.f : 0;
}

void AppWindow::OnDestroy()
{
	InputSystem::GetInstance()->SetCursorVisiable(true);

	Window::OnDestroy();
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
	c.world.SetIdentity();
	c.world.SetScale(tmpScale);

	//tmpMat.SetIdentity();
	//tmpMat.SetRotationZ(0);
	//c.world *= tmpMat;

	//tmpMat.SetIdentity();
	//tmpMat.SetRotationY(tmpRotY);
	//c.world *= tmpMat;

	//tmpMat.SetIdentity();
	//tmpMat.SetRotationX(tmpRotX);
	//c.world *= tmpMat;

	Matrix4x4 tmpWorldCamMat;


	tmpWorldCamMat.SetIdentity();

	tmpMat.SetIdentity();
	tmpMat.SetRotationX(tmpRotX);
	tmpWorldCamMat *= tmpMat;

	tmpMat.SetIdentity();
	tmpMat.SetRotationY(tmpRotY);
	tmpWorldCamMat *= tmpMat;

	Vector3 newPos = worldCamMat.GetTranslation() + tmpWorldCamMat.GetDirectionZ() * tmpForward * .0003f;
	newPos = newPos + tmpWorldCamMat.GetDirectionX() * tmpRight * .0003f;

	tmpWorldCamMat.SetTranslate(newPos);
	worldCamMat = tmpWorldCamMat;

	tmpWorldCamMat.Inverse();

	//c.view.SetIdentity();
	c.view = tmpWorldCamMat;

	//c.projection.SetOrthoLH
	//(
	//	(GetClientWindowRect().right - GetClientWindowRect().left) / 300.f,
	//	(GetClientWindowRect().bottom - GetClientWindowRect().top) / 300.f,
	//	-4.f, 4.f
	//);

	int width = GetClientWindowRect().right - GetClientWindowRect().left;
	int height = GetClientWindowRect().bottom - GetClientWindowRect().top;
	c.projection.SetPerpectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.f);

	pTmpCBuff->Update(GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext(), &c);
}

//void AppWindow::UpdateCamera()
//{
//	Matrix4x4 worldCamMat, tmp;
//
//	tmp.SetIdentity();
//	tmp.SetRotationX(tmpRotX);
//	worldCamMat *= tmp;
//
//	tmp.SetIdentity();
//	tmp.SetRotationY(tmpRotY);
//	worldCamMat *= tmp;
//
//	worldCamMat.Inverse();
//
//
//
//}

void AppWindow::OnMouseKeyDown(int mouseKey)
{
	switch (mouseKey)
	{
	case 0:
	{
		tmpScale = Vector3(0.6f, 1, 1);
		break;
	}
	case 1:
	{
		tmpScale = Vector3(1, 0.6f, 1);
		break;
	}
	case 2:
	{
		tmpScale = Vector3(1, 1, 0.6f);
		break;
	}
	default:
		break;
	}
}

void AppWindow::OnMouseKeyUp(int mouseKey)
{
	tmpScale = Vector3(1, 1, 1);
}

void AppWindow::OnMouseMove(const Point& mousePosition)
{
	//if (!deltaTime)
	//	MessageBox(hWnd, "WTF", "WTF", MB_OK);
	int width = GetClientWindowRect().right - GetClientWindowRect().left;
	int height = GetClientWindowRect().bottom - GetClientWindowRect().top;

	tmpRotX += InputSystem::GetInstance()->GetMouseDelta().y * .003f;
	tmpRotY += InputSystem::GetInstance()->GetMouseDelta().x * .003f;


	//InputSystem::GetInstance()->SetCursorPosition(Point(width / 2.f, height / 2.f));

}

void AppWindow::OnKeyDown(int keycode)
{
	if (keycode == 'W')
	{
		//tmpRotX += deltaTime;
		tmpForward = 1.f;
	}
	else if (keycode == 'S')
	{
		//tmpRotX -= deltaTime;
		tmpForward = -1.f;
	}
	else if (keycode == 'A')
	{
		//tmpRotY += deltaTime;
		tmpRight = -1.f;
	}
	else if (keycode == 'D')
	{
		//tmpRotY -= deltaTime;
		tmpRight = 1.f;
	}

}

void AppWindow::OnKeyUp(int keycode)
{
	tmpForward = 0;
	tmpRight = 0;
}
