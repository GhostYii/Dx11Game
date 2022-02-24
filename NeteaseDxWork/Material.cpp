#include "Material.h"
#include "GraphicsEngine.h"

#include <exception>

Material::Material(const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath) : textures()
{
	void* shaderByteCode = nullptr;
	UINT shaderSize = 0;
	GraphicsEngine::GetInstance()->GetRenderSystem()->CompileVertexShader(vertexShaderPath, "vert", &shaderByteCode, &shaderSize);
	pVertexShader = GraphicsEngine::GetInstance()->GetRenderSystem()->CreateVertexShader(shaderByteCode, shaderSize);

	if (!pVertexShader)
		throw std::exception("Create vertex shader failed!");

	GraphicsEngine::GetInstance()->GetRenderSystem()->CompilePixelShader(pixelShaderPath, "pixel", &shaderByteCode, &shaderSize);
	pPixelShader = GraphicsEngine::GetInstance()->GetRenderSystem()->CreatePixelShader(shaderByteCode, shaderSize);

	if (!pVertexShader)
		throw std::exception("Create pixel shader failed!");

	GraphicsEngine::GetInstance()->GetRenderSystem()->ReleaseCompiledShader();
}

Material::Material(const Material& other)
{
	pVertexShader = other.pPixelShader;
	pPixelShader = other.pVertexShader;
	pConstantBuffer = other.pConstantBuffer;

	cullMode = other.cullMode;
	textures = other.textures;
	
}

void Material::AddTexture(const TexturePtr& texture)
{	
	textures.push_back(texture);
}

void Material::EraseTexture(UINT index)
{
	if (index >= textures.size() || index < 0)
		return;

	textures.erase(textures.begin() + index);
}

void Material::SetCBuffer(void* cbuff, UINT bufferSize)
{
	if (!pConstantBuffer)
		pConstantBuffer = GraphicsEngine::GetInstance()->GetRenderSystem()->CreateConstantBuffer(cbuff, bufferSize);
	else
		pConstantBuffer->Update(GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext(), cbuff);
}

void Material::SetCullMode(D3D11_CULL_MODE mode)
{
	cullMode = mode;
}
