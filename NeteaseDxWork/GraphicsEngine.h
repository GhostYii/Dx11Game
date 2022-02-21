#pragma once
#include <d3d11.h>
#include "GraphicsClasses.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"

class GraphicsEngine
{
public:	
	RenderSystem* GetRenderSystem();
	TextureManager* GetTextureManger();
	MeshManager* GetMeshManager();

	void GetDefaultVertexShaderByteCodeAndSize(void** shaderByteCode, size_t* size);

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
	MeshManager* pMeshManager = nullptr;
	static GraphicsEngine* instance;

private:
	char pDefaultVertexShaderByteCode[1024];
	UINT defaultVertexShaderSize = 0;
	
};

