#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "Mesh.h"
#include "InputSystem.h"

#include<Windows.h>

#include "imgui_impl_win32.h"


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

	ImGui_ImplWin32_Init(hWnd);

	//InputSystem::GetInstance()->SetCursorVisiable(false);
	pTmpDayTexture = GraphicsEngine::GetInstance()->GetTextureManger()->CreateTextureFromFile(L"Assets\\Textures\\earth_day.jpg");
	pTmpNightTexture = GraphicsEngine::GetInstance()->GetTextureManger()->CreateTextureFromFile(L"Assets\\Textures\\earth_night.jpg");
	pTmpSkyboxTex = GraphicsEngine::GetInstance()->GetTextureManger()->CreateTextureFromFile(L"Assets\\Textures\\galaxy.jpg");
	pTmpMoonTex = GraphicsEngine::GetInstance()->GetTextureManger()->CreateTextureFromFile(L"Assets\\Textures\\moon.jpg");

	pTmpEarthMesh = GraphicsEngine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\lowpoly_earth.obj");
	pTmpSkyboxMesh = GraphicsEngine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	pTmpMoonMesh = GraphicsEngine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\sphere.obj");

	pSkySphereMateral = GraphicsEngine::GetInstance()->CreateMaterial(L"VertexShader.hlsl", L"DefaultSkyPS.hlsl");
	pSkySphereMateral->AddTexture(pTmpSkyboxTex);
	pSkySphereMateral->SetCullMode(D3D11_CULL_FRONT);

	pTmpEarthMaterial = GraphicsEngine::GetInstance()->CreateMaterial(L"VertexShader.hlsl", L"PixelShader.hlsl");
	pTmpEarthMaterial->AddTexture(pTmpDayTexture);
	pTmpEarthMaterial->AddTexture(pTmpNightTexture);
	pTmpEarthMaterial->SetCullMode(D3D11_CULL_BACK);
	
	pTmpMoonMaterial = GraphicsEngine::GetInstance()->CreateMaterial(L"MoonVS.hlsl", L"MoonPS.hlsl");
	pTmpMoonMaterial->AddTexture(pTmpMoonTex);
	pTmpMoonMaterial->SetCullMode(D3D11_CULL_BACK);

	RECT rect = this->GetClientWindowRect();
	pSwapChain = GraphicsEngine::GetInstance()->GetRenderSystem()->CreateSwapChain(this->hWnd, rect.right - rect.left, rect.bottom - rect.top);

	camTransMat.SetTranslation(Vector3(0, 0, -10.f));
}

void AppWindow::OnUpdate()
{
	InputSystem::GetInstance()->Update();

	ImGui_ImplWin32_NewFrame();
	Render();
}

void AppWindow::OnDestroy()
{
	InputSystem::GetInstance()->SetCursorVisiable(true);
	ImGui_ImplWin32_Shutdown();
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
	UpdateCamera();
	UpdateModel();
	UpdateSkybox();
}

void AppWindow::UpdateModel()
{
	// Scale - Rotation - Translate

	Constant earthBuffer;
	Matrix4x4 lightRotMat;
	lightRotMat.SetIdentity();

	lightRotMat.SetRotationY(tmpRotLightY);

	tmpRotLightY += .0707f * .001f;

	earthBuffer.world.SetIdentity();
	earthBuffer.view = camViewMat;
	earthBuffer.projection = camProjectionMat;
	earthBuffer.cameraPosition = camTransMat.GetTranslation();
	earthBuffer.light = lightRotMat.GetDirectionZ();

	pTmpEarthMaterial->SetCBuffer(&earthBuffer, sizeof(Constant));

	Constant moonCBuffer;
	moonCBuffer.world.SetIdentity();
	moonCBuffer.world.SetScale(Vector3(.5f, .5f, .5f));
	moonCBuffer.world.SetTranslation(Vector3(3.5f, 0, 0));
	moonCBuffer.view = camViewMat;
	moonCBuffer.projection = camProjectionMat;
	moonCBuffer.cameraPosition = camTransMat.GetTranslation();
	moonCBuffer.light = lightRotMat.GetDirectionZ();

	tmpMoonTrans = moonCBuffer.world.GetTranslation();

	pTmpMoonMaterial->SetCBuffer(&moonCBuffer, sizeof(Constant));
}

void AppWindow::UpdateSkybox()
{
	Constant cBuf;
	cBuf.world.SetIdentity();
	cBuf.world.SetScale(Vector3(100.f, 100.f, 100.f));
	cBuf.world.SetTranslation(camTransMat.GetTranslation());
	cBuf.view = camViewMat;
	cBuf.projection = camProjectionMat;

	pSkySphereMateral->SetCBuffer(&cBuf, sizeof(Constant));
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

	GraphicsEngine::GetInstance()->DrawMesh(pTmpSkyboxMesh, pSkySphereMateral);
	GraphicsEngine::GetInstance()->DrawMesh(pTmpEarthMesh, pTmpEarthMaterial);
	GraphicsEngine::GetInstance()->DrawMesh(pTmpMoonMesh, pTmpMoonMaterial);

	GraphicsEngine::GetInstance()->GetGuiManager()->Update();

	this->OnGUI();

	GraphicsEngine::GetInstance()->GetGuiManager()->Render();

	pSwapChain->Present(false);

	prevDeltaTime = newDeltaTime;
	newDeltaTime = GetTickCount();
	deltaTime = prevDeltaTime ? (newDeltaTime - prevDeltaTime) / 1000.f : 0;
}

void AppWindow::OnGUI()
{
	ImGui::Text("fps: %.1f", ImGui::GetIO().Framerate);
	ImGui::Text("cam pos: (%.2f, %.2f, %.2f)", camTransMat.GetTranslation().x, camTransMat.GetTranslation().y, camTransMat.GetTranslation().z);
	ImGui::Text("moon pos: (%.2f, %.2f, %.2f)", tmpMoonTrans.x, tmpMoonTrans.y, tmpMoonTrans.z);
	Matrix4x4 tmp;
	tmp.SetIdentity();
	tmp.SetRotationY(tmpRotLightY);
	ImGui::Text("sun direction : (%.2f, %.2f, %.2f)", tmp.GetDirectionZ().x, tmp.GetDirectionZ().y,tmp.GetDirectionZ().z);
}

void AppWindow::OnMouseKey(int mouseKey)
{
	
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
	//static int i = 0;
	//i++;

	//MessageBox(hWnd, std::to_string(i).c_str(), "pressed", MB_OK);
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
