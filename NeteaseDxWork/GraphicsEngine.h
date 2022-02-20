#pragma once
#include <d3d11.h>
#include "GraphicsClasses.h"
#include "RenderSystem.h"

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine() = default;

public:
	bool Init();
	bool Release();
	RenderSystem* GetRenderSystem();

	static GraphicsEngine* GetInstance();

private:
	RenderSystem* pRenderSystem = nullptr;
	
};

