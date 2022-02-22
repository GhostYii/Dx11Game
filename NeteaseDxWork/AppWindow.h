#pragma once
#include "Window.h"
#include "PinelineStruct.h"
#include "GraphicsEngine.h"
#include "InputSystem.h"

#include "Math.h"

class AppWindow : public Window, public InputListener
{
public:
	AppWindow() = default;
	virtual ~AppWindow() = default;

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
	virtual void OnMouseKeyUp(int mouseKey) override;

public:
	void WndUpdate();
	void UpdateModel();
	void UpdateSkybox();
	void UpdateCamera();

	void Render();	

private:
	SwapChainPtr pSwapChain = nullptr;

	bool isFullScreen = false;
private:
	VertexBufferPtr pTmpVB = nullptr;
	VertexShaderPtr pTmpVS = nullptr;
	PixelShaderPtr pTmpPS = nullptr;
	PixelShaderPtr pTmpSkyboxPS = nullptr;
	ConstantBufferPtr pTmpCBuff = nullptr;
	ConstantBufferPtr pTmpSkyboxCBuff = nullptr;
	IndexBufferPtr pTmpIndexBuff = nullptr;
	TexturePtr pTmpTexture = nullptr;
	TexturePtr pTmpSkyboxTex = nullptr;
	MeshPtr pTmpMesh = nullptr;
	MeshPtr pTmpSkyboxMesh = nullptr;

	unsigned long prevTime = 0;
	float prevDeltaTime = 0;
	float newDeltaTime = 0;
	float deltaTime = 0;
	
	float tmpPos = 0;
	float tmpDelta = 0;

	float tmpRotX = 0.f, tmpRotY = 0.f;
	Vector3 tmpScale = { 1,1,1 };

	float tmpForward = 0.f;
	float tmpRight = 0.f;

	Matrix4x4 camTransMat;
	Matrix4x4 camViewMat;
	Matrix4x4 camProjectionMat;

	float tmpRotLightY = 0;
	
	bool tmpIsMoveable = true;
};

