#pragma once
#include <d3d11.h>
#include "GraphicsClasses.h"
#include "RenderSystem.h"
#include "TextureManager.h"

class GraphicsEngine
{
public:	
	RenderSystem* GetRenderSystem();
	TextureManager* GetTextureManger();

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
	TextureManager* pTextureManager = nullptr;
	static GraphicsEngine* instance;
	
};

