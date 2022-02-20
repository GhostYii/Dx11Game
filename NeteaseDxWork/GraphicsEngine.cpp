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
}

void GraphicsEngine::Release()
{
	delete pRenderSystem;
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

GraphicsEngine* GraphicsEngine::GetInstance()
{
	return instance;
}
