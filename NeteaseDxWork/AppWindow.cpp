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
	Vector4 light;
	Vector4 cameraPosition;
};

void AppWindow::OnCreate()
{
	InputSystem::GetInstance()->AddListener(this);

	//InputSystem::GetInstance()->SetCursorVisiable(false);
	pTmpTexture = GraphicsEngine::GetInstance()->GetTextureManger()->CreateTextureFromFile(L"Assets\\Textures\\earth.jpg");
	pTmpSkyboxTex = GraphicsEngine::GetInstance()->GetTextureManger()->CreateTextureFromFile(L"Assets\\Textures\\galaxy.jpg");
	pTmpMesh = GraphicsEngine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	pTmpSkyboxMesh = GraphicsEngine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\skybox.obj");

	RECT rect = this->GetClientWindowRect();
	pSwapChain = GraphicsEngine::GetInstance()->GetRenderSystem()->CreateSwapChain(this->hWnd, rect.right - rect.left, rect.bottom - rect.top);

	camTransMat.SetTranslation(Vector3(0, 0, -10.f));

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

	GraphicsEngine::GetInstance()->GetRenderSystem()->CompilePixelShader(L"PixelShader.hlsl", "main", &shaderByteCode, &shaderSize);
	pTmpPS = GraphicsEngine::GetInstance()->GetRenderSystem()->CreatePixelShader(shaderByteCode, shaderSize);

	GraphicsEngine::GetInstance()->GetRenderSystem()->CompilePixelShader(L"DefaultSkyPS.hlsl", "main", &shaderByteCode, &shaderSize);
	pTmpSkyboxPS = GraphicsEngine::GetInstance()->GetRenderSystem()->CreatePixelShader(shaderByteCode, shaderSize);

	GraphicsEngine::GetInstance()->GetRenderSystem()->ReleaseCompiledShader();

	Constant cBuffer = { 0 };

	pTmpCBuff = GraphicsEngine::GetInstance()->GetRenderSystem()->CreateConstantBuffer(&cBuffer, sizeof(Constant));
	pTmpSkyboxCBuff = GraphicsEngine::GetInstance()->GetRenderSystem()->CreateConstantBuffer(&cBuffer, sizeof(Constant));
}

void AppWindow::OnUpdate()
{
	InputSystem::GetInstance()->Update();
	Render();
}

void AppWindow::OnDestroy()
{
	InputSystem::GetInstance()->SetCursorVisiable(true);

	Window::OnDestroy();
}

void AppWindow::OnSizeChanged()
{
	RECT rc = GetClientWindowRect();
	pSwapChain->Resize(rc.right, rc.bottom);
	OnUpdate();
}

void AppWindow::WndUpdate()
{
	// Scale - Rotation - Translate
	UpdateCamera();
	UpdateModel();
	UpdateSkybox();


	//Constant c = {};

	//tmpPos += deltaTime / 10.f;
	//if (tmpPos > 1.f)
	//	tmpPos = 0;

	//Matrix4x4 tmpMat;
	//Matrix4x4 lightRotMat;

	//lightRotMat.SetIdentity();
	//lightRotMat.SetRotationY(tmpRotLightY);

	//tmpRotLightY += .0707f * .003f;

	//c.light = lightRotMat.GetDirectionZ();

	//tmpDelta += deltaTime / .5f;

	//c.world.SetIdentity();
	//c.world.SetScale(tmpScale);

	//Matrix4x4 tmpWorldCamMat;


	//tmpWorldCamMat.SetIdentity();

	//tmpMat.SetIdentity();
	//tmpMat.SetRotationX(tmpRotX);
	//tmpWorldCamMat *= tmpMat;

	//tmpMat.SetIdentity();
	//tmpMat.SetRotationY(tmpRotY);
	//tmpWorldCamMat *= tmpMat;

	//Vector3 newPos = worldCamMat.GetTranslation() + tmpWorldCamMat.GetDirectionZ() * tmpForward * .0003f;
	//newPos = newPos + tmpWorldCamMat.GetDirectionX() * tmpRight * .0003f;

	//tmpWorldCamMat.SetTranslation(newPos);
	//c.cameraPosition = newPos;
	//worldCamMat = tmpWorldCamMat;

	//tmpWorldCamMat.Inverse();

	////c.view.SetIdentity();
	//c.view = tmpWorldCamMat;

	////c.projection.SetOrthoLH
	////(
	////	(GetClientWindowRect().right - GetClientWindowRect().left) / 300.f,
	////	(GetClientWindowRect().bottom - GetClientWindowRect().top) / 300.f,
	////	-4.f, 4.f
	////);

	//int width = GetClientWindowRect().right - GetClientWindowRect().left;
	//int height = GetClientWindowRect().bottom - GetClientWindowRect().top;
	//c.projection.SetPerpectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.f);

	//pTmpCBuff->Update(GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext(), &c);
}

void AppWindow::UpdateModel()
{
	Constant cBuf;
	Matrix4x4 lightRotMat;
	lightRotMat.SetIdentity();

	lightRotMat.SetRotationY(tmpRotLightY);

	tmpRotLightY += .0707f * .003f;

	cBuf.world.SetIdentity();
	cBuf.view = camViewMat;
	cBuf.projection = camProjectionMat;
	cBuf.cameraPosition = camTransMat.GetTranslation();
	cBuf.light = lightRotMat.GetDirectionZ();

	pTmpCBuff->Update(GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext(), &cBuf);
}

void AppWindow::UpdateSkybox()
{
	Constant cBuf;
	cBuf.world.SetIdentity();
	cBuf.world.SetScale(Vector3(100.f, 100.f, 100.f));
	cBuf.world.SetTranslation(camTransMat.GetTranslation());
	cBuf.view = camViewMat;
	cBuf.projection = camProjectionMat;

	// skybox cbuffer
	pTmpSkyboxCBuff->Update(GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext(), &cBuf);

}

void AppWindow::UpdateCamera()
{
	Matrix4x4 tmpWorldCamMat, tmp;
	tmpWorldCamMat.SetIdentity();

	tmp.SetIdentity();
	tmp.SetRotationX(tmpRotX);
	tmpWorldCamMat *= tmp;

	tmp.SetIdentity();
	tmp.SetRotationY(tmpRotY);
	tmpWorldCamMat *= tmp;

	Vector3 newPos = camTransMat.GetTranslation() + camTransMat.GetDirectionZ() * (tmpForward * .0005f);
	newPos = newPos + tmpWorldCamMat.GetDirectionX() * (tmpRight * .0005f);

	tmpWorldCamMat.SetTranslation(newPos);

	camTransMat = tmpWorldCamMat;

	tmpWorldCamMat.Inverse();

	camViewMat = tmpWorldCamMat;

	int width = GetClientWindowRect().right - GetClientWindowRect().left;
	int height = GetClientWindowRect().bottom - GetClientWindowRect().top;

	camProjectionMat.SetPerpectiveFovLH(1.57f, ((float)width / (float)height), .1f, 100.f);

}

void AppWindow::Render()
{
	GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext()->ClearRenderTargetColor(this->pSwapChain, 0, 0, 0, 1);

	RECT rect = GetClientWindowRect();
	GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext()->SetViewportSize(rect.right - rect.left, rect.bottom - rect.top);

	WndUpdate();

	GraphicsEngine::GetInstance()->GetRenderSystem()->SetRasterizerState(D3D11_CULL_BACK);
	GraphicsEngine::GetInstance()->DrawMesh(pTmpMesh, pTmpVS, pTmpPS, pTmpCBuff, pTmpTexture);

	GraphicsEngine::GetInstance()->GetRenderSystem()->SetRasterizerState(D3D11_CULL_FRONT);
	GraphicsEngine::GetInstance()->DrawMesh(pTmpSkyboxMesh, pTmpVS, pTmpSkyboxPS, pTmpSkyboxCBuff, pTmpSkyboxTex);

	pSwapChain->Present(false);

	prevDeltaTime = newDeltaTime;
	newDeltaTime = GetTickCount();
	deltaTime = prevDeltaTime ? (newDeltaTime - prevDeltaTime) / 1000.f : 0;
}

void AppWindow::OnKey(int keycode)
{
	if (!tmpIsMoveable)
		return;

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
	if (!tmpIsMoveable)
		return;

	int width = GetClientWindowRect().right - GetClientWindowRect().left;
	int height = GetClientWindowRect().bottom - GetClientWindowRect().top;

	tmpRotX += InputSystem::GetInstance()->GetMouseDelta().y * .003f;
	tmpRotY += InputSystem::GetInstance()->GetMouseDelta().x * .003f;
}

void AppWindow::OnKeyDown(int keycode)
{
}

void AppWindow::OnKeyUp(int keycode)
{
	tmpForward = 0;
	tmpRight = 0;

	if (keycode == VK_ESCAPE)
	{
		tmpIsMoveable = !tmpIsMoveable;
		InputSystem::GetInstance()->SetCursorVisiable(!tmpIsMoveable);
	}

	//if (keycode == VK_RETURN && InputSystem::GetInstance()->GetKey(VK_MENU))
	if (keycode == 'F')
	{
		isFullScreen = !isFullScreen;
		pSwapChain->SetFullScreen(isFullScreen);
	}
}
