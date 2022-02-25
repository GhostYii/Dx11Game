#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "Mesh.h"
#include "InputSystem.h"
#include "types.h"
#include "ModelObject.h"
#include "Transform.h"

#include <sstream>
#include<Windows.h>

#include "imgui_impl_win32.h"

void AppWindow::OnCreate()
{
	InputSystem::GetInstance()->AddListener(this);

	ImGui_ImplWin32_Init(hWnd);

	LoadModels();

	RECT rect = this->GetClientWindowRect();
	pSwapChain = GraphicsEngine::GetInstance()->GetRenderSystem()->CreateSwapChain(this->hWnd, rect.right - rect.left, rect.bottom - rect.top);

	camTransMat.SetTranslation(Vector3(0, 0, -10.f));

	deltaTime = timer.Mark();
}

void AppWindow::LoadModels()
{
	pEarthDayTex = GraphicsEngine::GetInstance()->GetTextureManger()->CreateTextureFromFile(L"Assets\\Textures\\earth_day.jpg");
	pEarthNightTex = GraphicsEngine::GetInstance()->GetTextureManger()->CreateTextureFromFile(L"Assets\\Textures\\earth_night.jpg");
	pSkysphereTex = GraphicsEngine::GetInstance()->GetTextureManger()->CreateTextureFromFile(L"Assets\\Textures\\galaxy.jpg");
	pMoonTex = GraphicsEngine::GetInstance()->GetTextureManger()->CreateTextureFromFile(L"Assets\\Textures\\moon.jpg");
	pSpaceshipTex = GraphicsEngine::GetInstance()->GetTextureManger()->CreateTextureFromFile(L"Assets\\Textures\\spaceship.jpg");

	pSphereMesh = GraphicsEngine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	pSpaceshipMesh = GraphicsEngine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\spaceship.obj");

	auto skyMaterial = GraphicsEngine::GetInstance()->CreateMaterial(L"VertexShader.hlsl", L"DefaultSkyPS.hlsl");
	skyMaterial->AddTexture(pSkysphereTex);
	skyMaterial->SetCullMode(D3D11_CULL_FRONT);

	auto earthMaterial = GraphicsEngine::GetInstance()->CreateMaterial(L"VertexShader.hlsl", L"PixelShader.hlsl");
	earthMaterial->AddTexture(pEarthDayTex);
	earthMaterial->AddTexture(pEarthNightTex);
	earthMaterial->SetCullMode(D3D11_CULL_BACK);

	auto moonMaterial = GraphicsEngine::GetInstance()->CreateMaterial(L"MoonVS.hlsl", L"MoonPS.hlsl");
	moonMaterial->AddTexture(pMoonTex);
	moonMaterial->SetCullMode(D3D11_CULL_BACK);

	auto spaceshipMaterial = GraphicsEngine::GetInstance()->CreateMaterial(L"VertexShader.hlsl", L"PixelShader.hlsl");
	spaceshipMaterial->AddTexture(pSpaceshipTex);
	spaceshipMaterial->SetCullMode(D3D11_CULL_BACK);

	TransformPtr skysphereTransform = std::make_shared<Transform>();
	skysphereTransform->SetScale(Vector3(camFar, camFar, camFar));
	ModelObjectPtr skysphere = std::make_shared<ModelObject>(skysphereTransform);
	skysphere->pMesh = pSphereMesh;
	skysphere->pMaterials.push_back(skyMaterial);
	modelsMap["skysphere"] = skysphere;

	TransformPtr earthTransform = std::make_shared<Transform>();
	earthTransform->SetPosition(Vector3(0, 0, 0));
	ModelObjectPtr earth = std::make_shared<ModelObject>(earthTransform);
	earth->pMesh = pSphereMesh;
	earth->pMaterials.push_back(earthMaterial);
	modelsMap["earth"] = earth;

	TransformPtr moonTransform = std::make_shared<Transform>();
	moonTransform->SetPosition(Vector3(5, 5, 0));
	ModelObjectPtr moon = std::make_shared<ModelObject>(moonTransform);
	moon->pMesh = pSphereMesh;
	moon->pMaterials.push_back(moonMaterial);
	modelsMap["moon"] = moon;

	TransformPtr spaceshipTransform = std::make_shared<Transform>();
	spaceshipTransform->SetPosition(Vector3(0, 0, 0));
	spaceshipTransform->SetScale(Vector3(.1f, .1f, .1f));
	ModelObjectPtr spaceship = std::make_shared<ModelObject>(spaceshipTransform);
	spaceship->pMesh = pSpaceshipMesh;
	spaceship->pMaterials.push_back(spaceshipMaterial);
	modelsMap["spaceship"] = spaceship;
}

void AppWindow::OnUpdate()
{
	InputSystem::GetInstance()->Update();

	ImGui_ImplWin32_NewFrame();

	Render();
}

void AppWindow::Render()
{
	GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext()->ClearRenderTargetColor(this->pSwapChain, 0, 0, 0, 1);

	RECT rect = GetClientWindowRect();
	GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext()->SetViewportSize(rect.right - rect.left, rect.bottom - rect.top);

	WndUpdate();

	for (auto iter = modelsMap.begin(); iter != modelsMap.end(); iter++)
	{
		iter->second->Draw();
	}

	GraphicsEngine::GetInstance()->GetGuiManager()->Update();

	this->OnGUI();

	GraphicsEngine::GetInstance()->GetGuiManager()->Render();

	pSwapChain->Present(false);
}

void AppWindow::WndUpdate()
{
	deltaTime = timer.Mark();
	UpdateCamera();
	UpdateModel();
	UpdateSkysphere();
}

void AppWindow::UpdateCamera()
{
	Matrix4x4 tmpWorldCamMat, tmp;
	tmpWorldCamMat.SetIdentity();

	tmp.SetIdentity();
	tmp.SetRotationX(camRotX);
	tmpWorldCamMat *= tmp;

	tmp.SetIdentity();
	tmp.SetRotationY(camRotY);
	tmpWorldCamMat *= tmp;

	Vector3 newPos = camTransMat.GetTranslation() + camTransMat.GetDirectionZ() * (camForward * deltaTime * fpsMoveSpeed);
	newPos = newPos + tmpWorldCamMat.GetDirectionX() * (camRight * deltaTime * fpsMoveSpeed);

	tmpWorldCamMat.SetTranslation(newPos);

	camTransMat = tmpWorldCamMat;

	tmpWorldCamMat.Inverse();

	camViewMat = tmpWorldCamMat;

	int width = GetClientWindowRect().right - GetClientWindowRect().left;
	int height = GetClientWindowRect().bottom - GetClientWindowRect().top;

	camProjectionMat.SetPerpectiveFovLH(camFov, ((float)width / (float)height), camNear, camFar);
}

void AppWindow::UpdateModel()
{
	// Scale - Rotation - Translate
	Matrix4x4 lightRotMat;
	lightRotMat.SetIdentity();
	
	lightRotMat.SetEuler(sunDir.x, sunDir.y, sunDir.z);

	for (auto iter = modelsMap.begin(); iter != modelsMap.end(); iter++)
	{
		Constant cBuf = {};
		cBuf.model.SetIdentity();
		cBuf.model.SetScale(iter->second->pTransform->GetScale());
		cBuf.model.SetTranslation(iter->second->pTransform->GetPosition());
		cBuf.view = camViewMat;
		cBuf.projection = camProjectionMat;
		cBuf.cameraPosition = camTransMat.GetTranslation();
		cBuf.light = lightRotMat.GetDirectionZ();

		iter->second->SetCBuffer(cBuf);
		iter->second->UpdateByCBuf();
	}

}

void AppWindow::UpdateSkysphere()
{
	modelsMap["skysphere"]->pTransform->SetPosition(camTransMat.GetTranslation());

	Constant skyphereCBuf;
	skyphereCBuf.model.SetIdentity();
	skyphereCBuf.model.SetScale(modelsMap["skysphere"]->pTransform->GetScale());
	skyphereCBuf.model.SetTranslation(modelsMap["skysphere"]->pTransform->GetPosition());
	skyphereCBuf.view = camViewMat;
	skyphereCBuf.projection = camProjectionMat;

	modelsMap["skysphere"]->SetCBuffer(skyphereCBuf);
	modelsMap["skysphere"]->UpdateByCBuf();
}

void AppWindow::OnGUI()
{
	ImGui::Begin("information");
	{
		ImGui::Text("fps: %.1f", ImGui::GetIO().Framerate);

		if (ImGui::CollapsingHeader("config"))
		{
			ImGui::SliderFloat("mouse sensitivity", &mouseSensitivity, 1.f, 100.f);
			ImGui::SliderFloat("move speed", &fpsMoveSpeed, 1.f, 100.f);
		}

		if (ImGui::CollapsingHeader("light"))
		{
			float position[3] = { to_deg<float>(sunDir.x), to_deg<float>(sunDir.y), to_deg<float>(sunDir.z) };
			ImGui::SliderFloat3("direction", position, 0, 360);
			sunDir.x = to_rad<float>(position[0]);
			sunDir.y = to_rad<float>(position[1]);
			sunDir.z = to_rad<float>(position[2]);
		}

		if (ImGui::CollapsingHeader("camera"))
		{
			float position[3];
			position[0] = camTransMat.GetTranslation().x;
			position[1] = camTransMat.GetTranslation().y;
			position[2] = camTransMat.GetTranslation().z;
			ImGui::DragFloat3("position##maincamerapos", position);

			ImGui::Checkbox("moveable", &isCamMoveable);
			ImGui::SameLine(100);
			ImGui::Checkbox("viewable", &isCamViewable);

			float deg = to_deg<float>(camFov);
			ImGui::SliderFloat("pov", &deg, 0, 180);

			camTransMat.SetTranslation(Vector3(position[0], position[1], position[2]));
			camFov = to_rad<float>(deg);

			ImGui::PushItemWidth(100);
			ImGui::DragFloat("near", &camNear);
			ImGui::SameLine();
			ImGui::DragFloat("far", &camFar);
			ImGui::PopItemWidth();
		}

		int index = 0;
		for (auto iter = modelsMap.begin(); iter != modelsMap.end(); ++iter)
		{
			if (iter->first == "skysphere")
				continue;

			std::ostringstream oss;
			oss << iter->first << " transform";
			if (ImGui::CollapsingHeader(oss.str().c_str()))
			{
				float position[3], euler[3], scale[3];

				position[0] = iter->second->pTransform->GetPosition().x;
				position[1] = iter->second->pTransform->GetPosition().y;
				position[2] = iter->second->pTransform->GetPosition().z;

				//euler[0] = iter->second->pTransform->GetEulerAngle().x;
				//euler[1] = iter->second->pTransform->GetEulerAngle().y;
				//euler[2] = iter->second->pTransform->GetEulerAngle().z;

				scale[0] = iter->second->pTransform->GetScale().x;
				scale[1] = iter->second->pTransform->GetScale().y;
				scale[2] = iter->second->pTransform->GetScale().z;

				if (iter->first == "spaceship")
					ImGui::BeginDisabled();
				oss.str("");
				oss << "position" << "##pos" << index;
				ImGui::DragFloat3(oss.str().c_str(), position);
				//ImGui::BeginDisabled();
				//oss.str("");
				//oss << "euler angle" << "##euler" << index;
				//ImGui::InputFloat3(oss.str().c_str(), euler);
				//ImGui::EndDisabled();
				oss.str("");
				oss << "scale" << "##scale" << index;
				ImGui::DragFloat3(oss.str().c_str(), scale);

				iter->second->pTransform->SetScale(Vector3(scale[0], scale[1], scale[2]));
				//iter->second->pTransform->SetRotation(Vector3(euler[0], euler[1], euler[2]));
				iter->second->pTransform->SetPosition(Vector3(position[0], position[1], position[2]));

				if (iter->first == "spaceship")
					ImGui::EndDisabled();
			}
			index++;
		}

		ImGui::End();
	}

}

void AppWindow::OnSizeChanged()
{
	RECT rc = GetClientWindowRect();
	pSwapChain->Resize(rc.right, rc.bottom);
	OnUpdate();
}

void AppWindow::OnDestroy()
{
	InputSystem::GetInstance()->SetCursorVisiable(true);
	ImGui_ImplWin32_Shutdown();
	Window::OnDestroy();
}

#pragma region Input Callback

void AppWindow::OnMouseKeyDown(int mouseKey)
{
}

void AppWindow::OnMouseKey(int mouseKey)
{

}

void AppWindow::OnMouseKeyUp(int mouseKey)
{
}

void AppWindow::OnMouseMove(const Point& mousePosition)
{
	if (!isCamViewable)
		return;

	int width = GetClientWindowRect().right - GetClientWindowRect().left;
	int height = GetClientWindowRect().bottom - GetClientWindowRect().top;

	camRotX += InputSystem::GetInstance()->GetMouseDelta().y * deltaTime * mouseSensitivity;
	camRotY += InputSystem::GetInstance()->GetMouseDelta().x * deltaTime * mouseSensitivity;
}

void AppWindow::OnKeyDown(int keycode)
{
}

void AppWindow::OnKey(int keycode)
{
	if (!isCamMoveable)
		return;

	if (keycode == 'W')
	{
		camForward = 1.f;
	}
	else if (keycode == 'S')
	{
		camForward = -1.f;
	}
	else if (keycode == 'A')
	{
		camRight = -1.f;
	}
	else if (keycode == 'D')
	{
		camRight = 1.f;
	}
}

void AppWindow::OnKeyUp(int keycode)
{
	camForward = 0;
	camRight = 0;

	if (keycode == VK_ESCAPE)
	{
		isCamMoveable = !isCamMoveable;
		isCamViewable = !isCamViewable;
	}
	else if (keycode == 'M')
	{
		isCamMoveable = !isCamMoveable;
	}
	else if (keycode == 'V')
	{
		isCamViewable = !isCamViewable;
	}

	if (keycode == VK_F11)
	{
		isFullScreen = !isFullScreen;
		pSwapChain->SetFullScreen(isFullScreen);
	}
}
#pragma endregion