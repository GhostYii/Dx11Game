#pragma once
#include <d3d11.h>
#include "GraphicsClasses.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "GUIManager.h"

class GraphicsEngine
{
public:	
	RenderSystem* GetRenderSystem();
	TextureManager* GetTextureManger();	
	MeshManager* GetMeshManager();
	GUIManager* GetGuiManager();

public:
	MaterialPtr CreateMaterial(const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath);

	void SetMaterial(const MaterialPtr& material);

public:
	void GetDefaultVertexShaderByteCodeAndSize(void** shaderByteCode, size_t* size);

	void DrawMesh(const MeshPtr& pMesh, const MaterialPtr& pMaterial);
	void DrawMesh(const MeshPtr& pMesh, const std::vector<MaterialPtr>& pMaterials);

	//void DrawObject(const)

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
	GUIManager* pGuiManager = nullptr;
	static GraphicsEngine* instance;

private:
	char pDefaultVertexShaderByteCode[1024];
	UINT defaultVertexShaderSize = 0;
	
};

