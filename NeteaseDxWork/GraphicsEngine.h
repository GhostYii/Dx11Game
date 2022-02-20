#pragma once
#include <d3d11.h>
#include "GraphicsClasses.h"
#include "RenderSystem.h"

class GraphicsEngine
{
public:	
	RenderSystem* GetRenderSystem();
	static GraphicsEngine* GetInstance();

	static void CreateInstance();
	static void ReleaseInstance();

private:
	GraphicsEngine();
	~GraphicsEngine();

private:
	void Init();
	void Release();

private:
	RenderSystem* pRenderSystem = nullptr;
	static GraphicsEngine* instance;
	
};

