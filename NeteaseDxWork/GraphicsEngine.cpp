#include "GraphicsEngine.h"
#include "PinelineStruct.h"
#include "ResourceStruct.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"

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
	// 先初始化GUIManager再初始化RenderSystem!
	try
	{
		pGuiManager = new GUIManager();
	}
	catch (...)
	{
		pGuiManager = nullptr;
	}

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
	pRenderSystem->CompileVertexShader(L"MeshDefaultVS.hlsl", "vert", &shaderByteCode, &sizeShader);
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

GUIManager* GraphicsEngine::GetGuiManager()
{
	return pGuiManager;
}

MaterialPtr GraphicsEngine::CreateMaterial(const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath)
{
	MaterialPtr ptr = nullptr;
	try 
	{
		ptr = std::make_shared<Material>(vertexShaderPath, pixelShaderPath);
	}
	catch (...) {}

	return ptr;
}

void GraphicsEngine::SetMaterial(const MaterialPtr& material)
{
	pRenderSystem->SetRasterizerState(material->cullMode);
	pRenderSystem->GetDeviceContext()->VSSetConstantBuffer(material->pConstantBuffer);
	pRenderSystem->GetDeviceContext()->PSSetConstantBuffer(material->pConstantBuffer);

	pRenderSystem->GetDeviceContext()->SetVertexShader(material->pVertexShader);
	pRenderSystem->GetDeviceContext()->SetPixelShader(material->pPixelShader);

	pRenderSystem->GetDeviceContext()->PSSetTexture(&material->textures[0], material->textures.size());

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

void GraphicsEngine::DrawMesh(const MeshPtr& pMesh, const MaterialPtr& pMaterial)
{
	SetMaterial(pMaterial);

	pRenderSystem->GetDeviceContext()->SetVertexBuffer(pMesh->GetVertexBuffer());
	pRenderSystem->GetDeviceContext()->SetIndexBuffer(pMesh->GetIndexBuffer());

	pRenderSystem->GetDeviceContext()->DrawIndexedTriangleList(pMesh->GetIndexBuffer()->GetIndexListSize(), 0, 0);
}

void GraphicsEngine::DrawMesh(const MeshPtr& pMesh, const std::vector<MaterialPtr>& pMaterials)
{
	pRenderSystem->GetDeviceContext()->SetVertexBuffer(pMesh->GetVertexBuffer());
	pRenderSystem->GetDeviceContext()->SetIndexBuffer(pMesh->GetIndexBuffer());

	for (size_t i = 0; i < pMesh->GetMaterialSlotCount(); i++)
	{
		if (i >= pMaterials.size())
			break;

		MaterialSlot slot = pMesh->GetMaterialSlot(i);
		SetMaterial(pMaterials[i]);
		pRenderSystem->GetDeviceContext()->DrawIndexedTriangleList(slot.numIndices, 0, slot.startIndex);
	}
}

GraphicsEngine* GraphicsEngine::GetInstance()
{
	return instance;
}
