#include "GraphicsEngine.h"

//#pragma comment(lib, "D3DCompiler.lib")

GraphicsEngine::GraphicsEngine()
{
}

bool GraphicsEngine::Init()
{
	pRenderSystem = new RenderSystem();
	pRenderSystem->Init();
	return true;
}

bool GraphicsEngine::Release()
{
	if (pRenderSystem)
		pRenderSystem->Release();
	
	return true;
}

RenderSystem* GraphicsEngine::GetRenderSystem()
{
	return pRenderSystem;
}

GraphicsEngine* GraphicsEngine::GetInstance()
{
	static GraphicsEngine engine;
	return &engine;
}
