#include "GraphicsEngine.h"

//#pragma comment(lib, "D3DCompiler.lib")

GraphicsEngine* GraphicsEngine::instance = nullptr;

GraphicsEngine::GraphicsEngine()
{
	Init();
}

GraphicsEngine::~GraphicsEngine()
{
	Release();
}

void GraphicsEngine::Init()
{	
	try
	{
		pRenderSystem = new RenderSystem();
	}
	catch (...) 
	{ 
		pRenderSystem = nullptr;
	}

	try
	{
		pTextureManager = new TextureManager();
	}
	catch (...)
	{
		pTextureManager = nullptr;
	}

	try
	{
		pMeshManager = new MeshManager();
	}
	catch (...)
	{
		pMeshManager = nullptr;
	}
	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;
	pRenderSystem->CompileVertexShader(L"MeshDefaultVS.hlsl", "main", &shaderByteCode, &sizeShader);
	memcpy(pDefaultVertexShaderByteCode, shaderByteCode, sizeShader);
	defaultVertexShaderSize = sizeShader;
	pRenderSystem->ReleaseCompiledShader();
}

void GraphicsEngine::Release()
{
	GraphicsEngine::instance = nullptr;
	delete pRenderSystem;
	delete pTextureManager;
	delete pMeshManager;
}

void GraphicsEngine::CreateInstance()
{
	if (GraphicsEngine::instance)
		delete GraphicsEngine::instance;

	GraphicsEngine::instance = new GraphicsEngine();
}

void GraphicsEngine::ReleaseInstance()
{
	if (!GraphicsEngine::instance)
		return;

	delete GraphicsEngine::instance;
}

RenderSystem* GraphicsEngine::GetRenderSystem()
{
	return pRenderSystem;
}

TextureManager* GraphicsEngine::GetTextureManger()
{
	return pTextureManager;
}

MeshManager* GraphicsEngine::GetMeshManager()
{
	return pMeshManager;
}

void GraphicsEngine::GetDefaultVertexShaderByteCodeAndSize(void** shaderByteCode, size_t* size)
{	
	*shaderByteCode = pDefaultVertexShaderByteCode;
	*size = defaultVertexShaderSize;
}

GraphicsEngine* GraphicsEngine::GetInstance()
{
	return instance;
}
