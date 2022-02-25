#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "Mesh.h"
#include "InputSystem.h"
#include "types.h"
#include "ModelObject.h"
#include "CameraObject.h"
#include "DirectionLightObject.h"
#include "Transform.h"

#include <sstream>
#include<Windows.h>

#include "imgui_impl_win32.h"

void AppWindow::OnCreate()
{
	InputSystem::GetInstance()->AddListener(this);

	ImGui_ImplWin32_Init(hWnd);

	CreateLight();
	CreateCamera();

	LoadModels();

	RECT rect = this->GetClientWindowRect();
	pSwapChain = GraphicsEngine::GetInstance()->GetRenderSystem()->CreateSwapChain(this->hWnd, rect.right - rect.left, rect.bottom - rect.top);

	deltaTime = timer.Mark();
}

void AppWindow::CreateCamera()
{
	RECT rect = this->GetClientWindowRect();
	pCamera = std::make_shared<CameraObject>((rect.right - rect.left) / 1.f / (rect.bottom - rect.top) / 1.f);	
	pCamera->pTransform->SetPosition(Vector3(0, 0, -60.f));
}

void AppWindow::CreateLight()
{
	Color lightcolor = Color();
	lightcolor.r = 1;
	lightcolor.g = 1;
	lightcolor.b = 1;
	lightcolor.a = 1;

	pLight = std::make_shared<DirectionLightObject>(1.5f, lightcolor);
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

	ModelObjectPtr skysphere = std::make_shared<ModelObject>();
	skysphere->pTransform->SetScale(Vector3(pCamera->farPlane, pCamera->farPlane, pCamera->farPlane));
	skysphere->pMesh = pSphereMesh;
	skysphere->pMaterials.push_back(skyMaterial);
	modelsMap["skysphere"] = skysphere;

	ModelObjectPtr earth = std::make_shared<ModelObject>();
	earth->SetScale(Vector3(50, 50, 50));
	earth->SetRotation(Vector3(0, 1.57f, 0));
	earth->SetPosition(Vector3(0, 0, 0));
	earth->pMesh = pSphereMesh;
	earth->pMaterials.push_back(earthMaterial);
	modelsMap["earth"] = earth;

	ModelObjectPtr moon = std::make_shared<ModelObject>();
	moon->SetScale(Vector3(5, 5, 5));
	moon->SetRotation(Vector3(0, 0, 0));
	moon->SetPosition(Vector3(200, 0, 0));
	moon->pMesh = pSphereMesh;
	moon->pMaterials.push_back(moonMaterial);
	modelsMap["moon"] = moon;

	ModelObjectPtr spaceship = std::make_shared<ModelObject>();
	spaceship->SetPosition(Vector3(50, 20, 20));
	spaceship->SetScale(Vector3(.2f, .2f, .2f));
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
	UpdateLight();
	UpdateModel();
	UpdateSkysphere();
}

void AppWindow::UpdateCamera()
{
	Vector3 euler = { to_deg<float>(camRotX),to_deg<float>(camRotY),0 };
	pCamera->pTransform->SetRotation(euler);

	Vector3 newPos = pCamera->pTransform->GetPosition() + pCamera->pTransform->GetMatrix().GetDirectionZ() * (camForward * deltaTime * fpCamMoveSpeed);
	newPos = newPos + pCamera->pTransform->GetMatrix().GetDirectionX() * (camRight * deltaTime * fpCamMoveSpeed);

	pCamera->pTransform->SetPosition(newPos);

	LONG width = GetClientWindowRect().right - GetClientWindowRect().left;
	LONG height = GetClientWindowRect().bottom - GetClientWindowRect().top;

	pCamera->UpdateVP(width / 1.f / height);
}

void AppWindow::UpdateLight()
{	
	pLight->pTransform->SetRotation(sunDir);
}

void AppWindow::UpdateModel()
{
	// Scale - Rotation - Translate
	for (auto iter = modelsMap.begin(); iter != modelsMap.end(); iter++)
	{
		Constant cBuf = {};
		cBuf.model.SetIdentity();
		cBuf.model.SetScale(iter->second->pTransform->GetScale());
		cBuf.model.SetEuler
		(
			iter->second->pTransform->GetEulerAngle()
		);
		cBuf.model.SetTranslation(iter->second->pTransform->GetPosition());
		cBuf.view = pCamera->GetViewMatrix();
		cBuf.projection = pCamera->GetProjectionMatrix();
		cBuf.cameraPosition = pCamera->pTransform->GetPosition();
		cBuf.light = pLight->Forward();
		cBuf.lightIntensity = pLight->intensity;
		cBuf.lightColor = pLight->color;

		iter->second->SetCBuffer(cBuf);
		iter->second->UpdateByCBuf();
	}

}

void AppWindow::UpdateSkysphere()
{
	modelsMap["skysphere"]->pTransform->SetPosition(pCamera->pTransform->GetPosition());
}

void AppWindow::OnGUI()
{
	ImGui::Begin("information");
	{
		ImGui::Text("fps: %.1f", ImGui::GetIO().Framerate);

		if (ImGui::CollapsingHeader("operator help"))
		{
			ImGui::Text("switch full screen: F11");
			ImGui::Text("toggle move: F");
			ImGui::Text("toggle view: V");
			ImGui::Text("toggle move & view: Esc");
		}

		if (ImGui::CollapsingHeader("config"))
		{
			ImGui::SliderFloat("mouse sensitivity", &mouseSensitivity, 1.f, 100.f);
			ImGui::SliderFloat("move speed", &fpCamMoveSpeed, 1.f, 100.f);
		}

		if (ImGui::CollapsingHeader("light"))
		{
			float position[3] = { to_deg<float>(sunDir.x), to_deg<float>(sunDir.y), to_deg<float>(sunDir.z) };
			ImGui::SliderFloat3("direction", position, 0, 360);
			sunDir.x = to_rad<float>(position[0]);
			sunDir.y = to_rad<float>(position[1]);
			sunDir.z = to_rad<float>(position[2]);

			ImGui::SliderFloat("intensity", &(pLight->intensity), 0, 10.f);

			float color[3] = { pLight->color.r,pLight->color.g,pLight->color.b };
			ImGui::ColorEdit3("color", color);
			pLight->color.r = color[0];
			pLight->color.g = color[1];
			pLight->color.b = color[2];
		}

		if (ImGui::CollapsingHeader("camera"))
		{
			float position[3];
			position[0] = pCamera->pTransform->GetPosition().x;
			position[1] = pCamera->pTransform->GetPosition().y;
			position[2] = pCamera->pTransform->GetPosition().z;
			ImGui::DragFloat3("position##maincamerapos", position);

			ImGui::Checkbox("moveable", &isCamMoveable);
			ImGui::SameLine(100);
			ImGui::Checkbox("viewable", &isCamViewable);

			float deg = to_deg<float>(pCamera->fov);
			ImGui::SliderFloat("pov", &deg, 1, 159);

			pCamera->pTransform->SetPosition(Vector3(position[0], position[1], position[2]));
			pCamera->fov = to_rad<float>(deg);

			ImGui::PushItemWidth(100);
			ImGui::DragFloat("near", &pCamera->nearPlane);
			ImGui::SameLine();
			ImGui::DragFloat("far", &pCamera->farPlane);
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

				euler[0] = to_deg<float>(iter->second->pTransform->GetEulerAngle().x);
				euler[1] = to_deg<float>(iter->second->pTransform->GetEulerAngle().y);
				euler[2] = to_deg<float>(iter->second->pTransform->GetEulerAngle().z);

				scale[0] = iter->second->pTransform->GetScale().x;
				scale[1] = iter->second->pTransform->GetScale().y;
				scale[2] = iter->second->pTransform->GetScale().z;

				//if (iter->first == "spaceship")
				//	ImGui::BeginDisabled();
				oss.str("");
				oss << "position" << "##pos" << index;
				ImGui::DragFloat3(oss.str().c_str(), position);
				//ImGui::BeginDisabled();
				oss.str("");
				oss << "euler angle" << "##euler" << index;
				ImGui::DragFloat3(oss.str().c_str(), euler);

				euler[0] = to_rad<float>(euler[0]);
				euler[1] = to_rad<float>(euler[1]);
				euler[2] = to_rad<float>(euler[2]);

				//ImGui::EndDisabled();
				oss.str("");
				oss << "scale" << "##scale" << index;
				ImGui::DragFloat3(oss.str().c_str(), scale);

				iter->second->pTransform->SetScale(Vector3(scale[0], scale[1], scale[2]));
				iter->second->pTransform->SetRotation(Vector3(euler[0], euler[1], euler[2]));
				iter->second->pTransform->SetPosition(Vector3(position[0], position[1], position[2]));

				//if (iter->first == "spaceship")
				//	ImGui::EndDisabled();
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