#pragma once
#include "Window.h"
#include "PinelineStruct.h"
#include "GraphicsEngine.h"
#include "InputSystem.h"
#include "ComponentClass.h"
#include "timer.h"

#include "Math.h"

class AppWindow : public Window, public InputListener
{
public:
	AppWindow() = default;
	virtual ~AppWindow() = default;

public:
	void WndUpdate();
	void UpdateLight();
	void UpdateModel();
	void UpdateSkysphere();
	void UpdateCamera();

	void Render();	

protected:
	virtual void OnGUI();

	// Inherited via Window
	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnDestroy() override;
	virtual void OnSizeChanged() override;

	// Inherited via InputLisenter
	virtual void OnKeyDown(int keycode) override;
	virtual void OnKey(int keycode) override;
	virtual void OnKeyUp(int keycode) override;
	virtual void OnMouseMove(const Point& mousePosition) override;
	virtual void OnMouseKeyDown(int mouseKey) override;
	virtual void OnMouseKey(int mouseKey) override;
	virtual void OnMouseKeyUp(int mouseKey) override;
	virtual void OnMouseWheel(int delta) override;

private:
	void CreateCamera();
	void CreateLight();
	void LoadModels();

private:
	SwapChainPtr pSwapChain = nullptr;

	Timer timer;

	bool isFullScreen = false;
	bool isCamMoveable = true;
	bool isCamViewable = true;
	bool isUIShow = true;

	CameraObjectPtr pCamera;
	TPCameraObjectPtr pTpCamera;
	DirectionLightObjectPtr pLight;
	std::unordered_map<std::string, ModelObjectPtr> modelsMap;

	float deltaTime = 0;

	float mouseSensitivity = 8.f;
	float fpCamMoveSpeed = 35.f;

	TexturePtr pEarthDayTex = nullptr;
	TexturePtr pEarthNightTex = nullptr;
	TexturePtr pSkysphereTex = nullptr;
	TexturePtr pMoonTex = nullptr;
	TexturePtr pSpaceshipTex = nullptr;

	MeshPtr pSphereMesh = nullptr;
	MeshPtr pSpaceshipMesh = nullptr;

private:
	Vector3 camRotation;
	float camForward = 0.f;
	float camRight = 0.f;

	Vector3 sunDir;

	bool isFpsMode = true;
	bool isHelperShow = true;
};

